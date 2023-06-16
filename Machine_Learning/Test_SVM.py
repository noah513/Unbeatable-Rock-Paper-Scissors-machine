import time
import numpy as np
from sklearn.svm import SVC
import pickle
import sys

def read_file(filename):
    with open(filename, 'r') as file:
        while True:
            file.seek(0, 2)
            line = file.readline()
            if (not line) or (line == '\n'):
                time.sleep(0.001)
                continue
            yield line

svm = pickle.load(open('SVM.pkl', 'rb'))

while True:
    with open('result.txt', 'r') as result_file:
        result_line = result_file.readline()
        if result_line == '1\n':
            break
            
# keyin = input()
print("bbbbbb")
file_reader = read_file('test.txt')
while True :
    for line in file_reader:
        values = line.split()
        X_test = [values]
        result = svm.predict(X_test)[0]
        if result == 0 :
            with open('result.txt', 'w') as file:
                file.write("R\n")
                print("R")
        elif result == 1 :
            with open('result.txt', 'w') as file:
                file.write("P\n")
                print("P")
        elif result == 2 :
            with open('result.txt', 'w') as file:
                file.write("S\n")
                print("S")
        print(result)
        predict_lst = []
        while True:
            with open('result.txt', 'r') as result_file:
                result_line = result_file.readline()
                if result_line == '1\n':
                    break
