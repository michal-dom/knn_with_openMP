#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <math.h>
#include <cfloat>

#include "Utils.h"
#include "StandScaler.h"
#include "NormScaler.h"

#define NUM_THREADS 2
#define ROWS 8000
#define COLS 784

using namespace std;


const long ARRAY_SIZE = 60000 * COLS;

const long DATA_SIZE = 8000 * 784;
const long TEST_SIZE = 2000 * 784;



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

int main() {



    int * data = new int[ARRAY_SIZE];
    auto * normal = new double[ARRAY_SIZE];
    auto * classesTrain = new int[60000];
//    auto * classesTest = new int[2000];


    readUnprocessed(data, classesTrain);

    normalize(data, normal);
    print_arr(normal);
//    readData(test, "norm_test.csv");
//    readClasses(classesTrain, "classes_train.txt");
//    readClasses(classesTest, "classes_test.txt");


//    print_arr(data);
//    print_arr(test);

//    double startTime = omp_get_wtime();
//    knn(data, test, classesTrain, classesTest);
//    double time = omp_get_wtime() - startTime;

//    cout << "Time: " << time << "\n";





    return 0;
}
