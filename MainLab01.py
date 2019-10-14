# author Wojciech Jakub Jargielo & Michal Domagała

import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.neighbors import KNeighborsClassifier
from sklearn.preprocessing import MinMaxScaler
from sklearn.preprocessing import StandardScaler

def readcsv(filename):
    data = pd.read_csv(filename)
    return np.array(data)

def standarization(data):
    scaler = StandardScaler()
    scaler.fit(data)
    print(scaler.mean_)
    return scaler.transform(data)

def minMax(data):
    scaler = MinMaxScaler()
    scaler.fit(data)
    print(scaler.data_max_)
    return scaler.transform(data)

def KNN(X, y):
    neigh = KNeighborsClassifier(n_neighbors=1)
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.33)
    neigh.fit(X_train, y_train)
    score = neigh.score(X_test, y_test)
    print(score)

if __name__ == "__main__":
    data = readcsv("mnist_train.csv")
    y = 'classes'  # to do - get classes from csv
    KNN(standarization(data), y)
    KNN(minMax(data), y)