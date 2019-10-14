import numpy as np
import pandas as pd
from sklearn.preprocessing import MinMaxScaler
from sklearn.preprocessing import StandardScaler


def readcsv(filename):
    data = pd.read_csv(filename)
    return np.array(data)


def standaryzacja(csv_data):
    scaler = StandardScaler()
    print(scaler.fit(csv_data))
    print(scaler.mean_)
    print(scaler.transform(csv_data))


def minMax(data):
    scaler = MinMaxScaler()
    print(scaler.fit(csv_data))
    print(scaler.data_max_)
    print(scaler.transform(data))


if __name__ == "__main__":
    csv_data = readcsv('mnist_train.csv');
    print("-------standaryzacja--------------")
    standaryzacja(csv_data)
    print("-------minMax--------------")
    minMax(csv_data)
