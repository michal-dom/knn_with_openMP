#include <iostream>
#include <fstream>
#include <sstream>
#include <omp.h>
#include <stdio.h>
#include <math.h>
#include "Utils.h"

#define NUM_THREADS 3

using namespace std;

int ROWS = 60000;
const int COLS = 784;
const long ARRAY_SIZE = ROWS * COLS;

const int ROWS_TEST = 1000;
const long TEST_SIZE = ROWS * COLS;

int min(const int col, const int * array){
    int min_val = 255;
    #pragma omp parallel for reduction(min: min_val) num_threads(NUM_THREADS)
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

    #pragma omp parallel for reduction(max: max_val) num_threads(NUM_THREADS)
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

    #pragma omp parallel for num_threads(NUM_THREADS) shared(normalizedArray)
    for(int i = 0; i < ROWS; i++){
        int index = (i*COLS) + col;
        if(denominator == 0){
            normalizedArray[index] = 0;
        } else {
            normalizedArray[index] = (array[index] - min_val) / denominator;
        }
    }
}

double mean(const int col, const int * array){
    double sum = 0;

    #pragma omp parallel for reduction (+:sum) num_threads(NUM_THREADS)
    for(int i = 0; i < ROWS; i++){
        int index = (i*COLS) + col;
        sum += array[index];
    }

    return sum / ROWS;
}

double deviation(int col, const int * array){
    double sum = 0;
    const double mean_val = mean(col, array);

    #pragma omp parallel for reduction(+:sum) num_threads(NUM_THREADS)
    for(int i = 0; i < ROWS; i++){
        int index = (i*COLS) + col;
        sum += (array[index] - mean_val) * (array[index] - mean_val);
    }
    double variance = sum / ROWS;
    return sqrt(variance);
}

void standardization(int col, const int * array, double * standardizedArray){
    const double mean_val = mean(col, array);
    const double deviation_val = deviation(col, array);

    #pragma omp parallel for num_threads(NUM_THREADS)
    for(int i = 0; i < ROWS; i++){

        int index = (i*COLS) + col;
        if(deviation_val == 0){
            standardizedArray[index] = 0;
        } else{
            standardizedArray[index] = (array[index] - mean_val) / deviation_val;

        }
    }

}

int euclidian(const double * array, const double * test_array, const int * classes){

    double min_dist = 10000.0;
    int best_class = 0;

//    #pragma omp parallel for num_threads(NUM_THREADS) reduction(min: min_dist, best_class)
    for(int i = 0; i < ROWS; i++){
        double sum = 0;
//        #pragma omp parallel for reduction (+:sum) num_threads(NUM_THREADS)
        for (int j = 0; j < COLS; j++) {
            int index = (i*COLS) + j;
            sum += (array[index] - test_array[j]) * (array[index]  - test_array[j]);
        }
        double tmp = sqrt(sum);
        if(tmp < min_dist){
            min_dist = tmp;
            best_class = classes[i];
        }
    }
    return best_class;

}




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

void classify(double * standardizedData, double * test2D[ROWS_TEST], int * classesData, int * classesTest){

    ROWS = 2000;
    int hits = 0;

    #pragma omp parallel for num_threads(NUM_THREADS) reduction(+:hits)
    for (int i = 0; i < ROWS_TEST; ++i) {
        double * tmp = test2D[i];
        int pred = euclidian(standardizedData, tmp, classesData);
        if(pred == classesTest[i]){
            hits++;
        }
    }

    double score = (double) hits / ROWS_TEST;

//    cout << score << endl;
}
void writeFile(const double * array){
    ofstream file;
    file.open("processed.csv");

    long k = 0;
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            file << array[k] << ",";
            k++;
        }
        file << endl;
    }
    file.close();
}

void readTrainData(double * array){
    ifstream input("processed_minMax.csv");

    int j = 0;
    int i = 0;
    for(string line; getline( input, line ); ){
        stringstream stream(line.substr(0, line.size() - 1));
        string digit;
        while( getline(stream, digit, ',') ) {
            array[j] = strtod(digit.c_str(), nullptr);
            j++;
        }
        i++;
    }

    input.close();
}


void saveClasses(int * array){
    ofstream file;
    file.open("classes.txt");

    for(int i = 0; i < ROWS; i++){
        file << array[i] << "\n";
    }
    file.close();
}



int main() {


    auto * data = new int[ARRAY_SIZE];
    auto * normalizedData = new double[ARRAY_SIZE];
    auto * standardizedData = new double[ARRAY_SIZE];
    auto * classesData = new int[ROWS];


    auto * test = new int[TEST_SIZE];
    auto * normalizedTest = new double[TEST_SIZE];
    auto * standardizedTest = new double[TEST_SIZE];
    auto * classesTest = new int[ROWS_TEST];



//    readData(data, classesData);
//    print_arr(data);
//    svaeClasses(classesData);
//    readTest(test, classesTest);

//    print_arr(data);




//    standarize(data, standardizedData);
//    writeFile(standardizedData);
//    ROWS = ROWS_TEST;
//    standarize(test, standardizedTest);

//    double * test2D[ROWS_TEST];






    return 0;
}
