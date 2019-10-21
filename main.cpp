#include <iostream>
#include <fstream>
#include <sstream>
#include <omp.h>
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


const long ARRAY_SIZE = ROWS * COLS;

const long DATA_SIZE = 8000 * 784;
const long TEST_SIZE = 2000 * 784;



void normalize(const int * array, double * normalizedArray){

    #pragma omp parallel for num_threads(NUM_THREADS) shared(normalizedArray)
    for(int i = 0; i < COLS; i++){
        minMaxNormalization(i, array, normalizedArray);
    }
}

void standarize(const int * array, double * normalizedArray){

    #pragma omp parallel for num_threads(NUM_THREADS) shared(normalizedArray)
    for(int i = 0; i < COLS; i++){
        standardization(i, array, normalizedArray);
    }
}

int euclidian(double * train, double * test, int testRow){

    auto minVal = DBL_MAX;
    int classIndex = 0;
    #pragma omp parallel for num_threads(NUM_THREADS) shared(classIndex) reduction(min: minVal)
    for(int i = 0; i < ROWS; i++){
        double sum = 0.0;
        #pragma omp parallel for num_threads(NUM_THREADS) reduction(+: sum)
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
    #pragma omp parallel for reduction(+: hits) num_threads(NUM_THREADS) shared(train, test, classesTrain, classesTest)
    for(int i = 0; i < 2000; i++){
        int predClass = classesTrain[euclidian(train, test, i)];
        if(predClass == classesTest[i]){
            hits++;
        }
    }

    auto accuracy = (double) hits;
    accuracy = accuracy/20;

    cout << "Classifier accuracy is " << accuracy << "%." << endl;

}

int main() {


    auto * data = new double[DATA_SIZE];
    auto * test = new double[TEST_SIZE];
    auto * classesTrain = new int[8000];
    auto * classesTest = new int[2000];


    readData(data, "norm_train.csv");
    readData(test, "norm_test.csv");
    readClasses(classesTrain, "classes_train.txt");
    readClasses(classesTest, "classes_test.txt");

//    print_arr(data);
//    print_arr(test);

    double startTime = omp_get_wtime();
    knn(data, test, classesTrain, classesTest);
    double time = omp_get_wtime() - startTime;

    cout << "Time: " << time << "\n";





    return 0;
}
