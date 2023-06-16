#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    struct addrinfo hints, *res;
    int sockfd, status;
    char buffer[1024];

    // Set up socket parameters
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // Resolve the IP address of the remote server using getaddrinfo
    status = getaddrinfo("192.168.124.224", "5000", &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    // Create a socket object and connect to the remote server
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket error");
        exit(1);
    }

    status = connect(sockfd, res->ai_addr, res->ai_addrlen);
    if (status == -1) {
        perror("connect error");
        exit(1);
    }

    // Send a message to the server
    FILE *file = fopen("result.txt", "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }
    
    char msg[1024];
    while(fgets(msg,1024,file)==NULL){
        rewind(file);
    }
    fclose(file);

    file=fopen("result.txt", "r");
    if (fgets(msg, sizeof(msg), file) == NULL) {
        printf("Failed to read the file1.\n");
        fclose(file);
        return 1;
    }
    
    fclose(file);
    const char *message = msg;
    send(sockfd, message, strlen(message), 0);

    // Receive the response from the server
    int bytes_received = recv(sockfd, buffer, sizeof buffer - 1, 0);
    buffer[bytes_received] = '\0';

    // Print the response from the server
    printf("%s", buffer);
    file = fopen("result.txt", "w");
    fclose(file);
    // Close the socket connection
    close(sockfd);

    return 0;
}