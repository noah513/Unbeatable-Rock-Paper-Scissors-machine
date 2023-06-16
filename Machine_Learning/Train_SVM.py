from sklearn.svm import SVC
import pickle

X_train = []
y_train = []
svm = SVC()

with open('train.txt', 'r') as file:
    for line in file:
        values = line.split()
        x_values = [float(val) for val in values[:2]]
        y_value = int(values[2])
        X_train.append(x_values)
        y_train.append(y_value)

svm.fit(X_train, y_train)
pickle.dump(svm, open('SVM.pkl', 'wb'))
