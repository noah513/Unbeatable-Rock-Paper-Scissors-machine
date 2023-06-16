#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
    int sockfd;
    char choice[100]; 
} Client;

void *client_handling(void *arg) {
    Client *ci = arg;

    int bytes_received = recv(ci->sockfd, ci->choice, sizeof(ci->choice) - 1, 0);
    ci->choice[bytes_received] = '\0';
    printf("Received message: %s \n", ci->choice);
    
    return NULL;
}

void compute(Client *clients, int num_clients) {
    char response[2];

    int r_count = 0;
    int p_count = 0;
    int s_count = 0;

    for (int i = 0; i < num_clients; i++)
    {
        char client_select = clients[i].choice[0];
        if (client_select == 'R') {r_count++;}
        else if (client_select == 'P') {p_count++;}
        else if (client_select == 'S') {s_count++;}
        else {exit(1);}
    }

    if (num_clients == 1) {
        if (r_count == 1) {
            response[0] = 'P';
        }
        else if (p_count == 1) {
            response[0] = 'S';
        }
        else {
            response[0] = 'R';
        }
    }
    else if (num_clients == 2) {
        if (r_count == 2 || (r_count == 1 && p_count == 1)) {
            response[0] = 'P';
        }
        else if (p_count == 2 || (p_count == 1 && s_count == 1)) {
            response[0] = 'S';
        }
        else if (s_count == 2 || (s_count == 1 && r_count == 1)) {
            response[0] = 'R';
        }
        else {
            response[0] = 'P';
        }
    }
    else {
        if (r_count == 3 || (r_count == 2 && p_count == 1) || (p_count == 2 && r_count == 1)) {
            response[0] = 'P';
        }
        else if (p_count == 3 || (p_count == 2 && s_count == 1) || (s_count == 2 && p_count == 1)) {
            response[0] = 'S';
        }
        else if (s_count == 3 || (s_count == 2 && r_count == 1) || (r_count == 2 && s_count == 1)) {
            response[0] = 'R';
        }
        else {
            response[0] = 'P';
        }
    }
    response[1] = '\0';

    printf("Response: %s \n", response);

    FILE *fpointer = fopen("Display.txt", "w");
    if (fpointer == NULL) {
        perror("File open error");
        exit(1);
    }
    fprintf(fpointer,"%s\n",response);
    fclose(fpointer);
}

int main(int argc, char *argv[]) {
    struct addrinfo hints, *res;
    int sockfd, clientfd, status;
    Client clients[3];
    pthread_t p_threads[3];
    int num_clients = 0;

    int max_clients = 3;

    if (argc > 1) {
        int argument = atoi(argv[1]);

        if (argument >= 1 && argument <= 3) {
            max_clients = argument;
        }
        else {
            fprintf(stderr, "Error: Argument must be 1, 2, or 3.\n");
            exit(1);
        }
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfo(NULL, "5000", &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket error");
        exit(1);
    }

    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse);

    status = bind(sockfd, res->ai_addr, res->ai_addrlen);
    if (status == -1) {
        perror("bind error");
        exit(1);
    }

    status = listen(sockfd, 3);
    if (status == -1) {
        perror("listen error");
        exit(1);
    }

    printf("Listening on port 5000...\n");

    while (num_clients < max_clients) {
        clientfd = accept(sockfd, NULL, NULL);
        if (clientfd == -1) {
            perror("accept error");
            continue;
        }

        printf("Accepted connection from client\n");

        clients[num_clients].sockfd = clientfd;

        num_clients++;
    }
    num_clients = 0;
    printf("Press Enter to send signal\n");
    getchar();
    while (num_clients < max_clients) {
        Client *ci = &clients[num_clients];
        const char *response = "1\n";
        send(ci->sockfd, response, strlen(response), 0);
        printf("Sent signal to client\n");

        num_clients++;
    }
    num_clients = 0;
    while (num_clients < max_clients) {
        pthread_create(&p_threads[num_clients], NULL, client_handling, &clients[num_clients]);

        num_clients++;
    }

    for (int i = 0; i < max_clients; i++) {
        pthread_join(p_threads[i], NULL);
    }

    compute(clients, max_clients);

    for (int i = 0; i < max_clients; i++) {
        close(clients[i].sockfd);
    }

    freeaddrinfo(res);
    
    close(sockfd);

    return 0;
}