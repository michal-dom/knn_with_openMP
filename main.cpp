#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <math.h>
#include <cfloat>
#include <mpi.h>


//#include "Utils.h"
//#include "StandScaler.h"
//#include "NormScaler.h"

#define NUM_THREADS 2
#define ROWS 8000
#define COLS 784

using namespace std;


const long ARRAY_SIZE = 60000 * COLS;

const long DATA_SIZE = 8000 * 784;
const long TEST_SIZE = 2000 * 784;

int min(const int col, const int * array){
    int min_val = 255;

    for(int i = 0; i < ROWS; i++){
        int index = (i*COLS) + col;
        if (min_val > array[index]){
            min_val = array[index];
        }
    }
    return min_val;
}

int max(const int col, const int * array){
    int max_val = 0;

    for(int i = 0; i < ROWS; i++){
        int index = (i*COLS) + col;
        if (max_val < array[index]){
            max_val = array[index];
        }
    }
    return max_val;
}

void minMaxNormalization(const int col, const int * array, double * normalizedArray){

    const int min_val = min(col, array);
    const int max_val = max(col, array);
    const double denominator = max_val - min_val;

    for(int i = 0; i < ROWS; i++) {
        int index = (i * COLS) + col;
        if (denominator == 0) {
            normalizedArray[index] = 0;
        } else {
            normalizedArray[index] = (array[index] - min_val) / denominator;
        }
    }
}

void normalize(const int * array, double * normalizedArray){

    int part = COLS/NUM_THREADS;

    MPI_Init(NULL, NULL);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    for(int i = 0; i < part; i++){
        int index = i + (part * world_rank);
        minMaxNormalization(index, array, normalizedArray);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();


}

void standarize(const int * array, double * normalizedArray){

    for(int i = 0; i < COLS; i++){
        standardization(i, array, normalizedArray);
    }
}

int manhattan(double * train, double * test, int testRow){

    auto minVal = DBL_MAX;
    auto * voting = new double[10];

    int classIndex = 0;
    for(int i = 0; i < ROWS; i++){
        double sum = 0.0;
        for(int j = 0; j < COLS; j++){
            int indexTrain = 784*i + j;
            int indexTest = 784*testRow + j;
            sum += fabs(train[indexTrain] - test[indexTest]);
        }
        if(minVal > sum){
            minVal = sum;
            classIndex = i;
        }
    }
    return classIndex;
}

int chebyshev(double * train, double * test, int testRow){

    auto minVal = DBL_MAX;
    int classIndex = 0;
    for(int i = 0; i < ROWS; i++){
        double maxVal = 0.0;

        for(int j = 0; j < COLS; j++){
            int indexTrain = 784*i + j;
            int indexTest = 784*testRow + j;

            double tmp = fabs(train[indexTrain] - test[indexTest]);
            if(maxVal < tmp) maxVal = tmp;
        }
        if(minVal > maxVal){
            minVal = maxVal;
            classIndex = i;
        }
    }
    return classIndex;
}



int euclidian(double * train, double * test, int testRow){

    auto minVal = DBL_MAX;
    int classIndex = 0;
    for(int i = 0; i < ROWS; i++){
        double sum = 0.0;
        for(int j = 0; j < COLS; j++){
            int indexTrain = 784*i + j;
            int indexTest = 784*testRow + j;
            sum += (train[indexTrain] - test[indexTest]) * (train[indexTrain] - test[indexTest]);
        }
        sum = sqrt(sum);
        if(minVal > sum){
            minVal = sum;
            classIndex = i;
        }
    }
    return classIndex;
}

void knn(double * train, double * test, const int * classesTrain, const int * classesTest){


    auto hits = 0;
    for(int i = 0; i < 2000; i++){
        int predClass = classesTrain[chebyshev(train, test, i)];
        if(predClass == classesTest[i]){
            hits++;
        }
    }

    auto accuracy = (double) hits;
    accuracy = accuracy/20;

    cout << "Classifier accuracy is " << accuracy << "%." << endl;

}

void readUnprocessed(int * array, int * classes){
    ifstream input("mnist_train.csv");

    string foo;
    getline( input, foo );

    int j = 0;
    int i = 0;
    for(string line; getline( input, line ); ){
        stringstream stream(line.substr(0, line.size() - 1));
        string digit;
        getline(stream, digit, ',');
        classes[i] = strtol(digit.c_str(), nullptr, 0);
        while( getline(stream, digit, ',') ) {
            array[j] = strtol(digit.c_str(), nullptr, 0);
            j++;
        }
        i++;
    }

    input.close();
}

void print_arr(double * array){
    long k = 0;
    for(int i = 0; i < 60000; i++){
        for(int j = 0; j < 784; j++){
            cout << array[k] << " ";
            k++;
        }
        cout << endl;
    }
}

int main() {



    int * data = new int[ARRAY_SIZE];
    auto * normal = new double[ARRAY_SIZE];
    auto * classesTrain = new int[60000];



    readUnprocessed(data, classesTrain);

    normalize(data, normal);
    print_arr(normal);





    return 0;
}
