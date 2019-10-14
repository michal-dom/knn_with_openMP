#include <iostream>
#include <fstream>
#include <sstream>
#include <omp.h>
#include <stdio.h>
#include <math.h>
#define NUM_THREADS 4

using namespace std;


const int ROWS = 60000;
const int COLS = 784;
const long ARRAY_SIZE = ROWS * COLS;


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

void print_col(int col, int * array){
    for(int i = 0; i < ROWS; i++){
        int index = (i*COLS) + col;
        if(array[index] > 0){
            cout << array[index] << endl;
        }

    }
}


void print_arr(double * array){
    long k = 0;
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            cout << array[k] << " ";
            k++;
        }
        cout << endl;
    }
}

int euclidian(const double * array, const double * test_array, const int * classes){
//    double sum = 0;
//    auto * votes = new double[10];
    auto * distances = new double[ROWS];


    double distance = 100000.0;

//    #pragma omp parallel for reduction (+:sum) collapse(2) num_threads(4)
    #pragma omp parallel for num_threads(NUM_THREADS) shared(distances)
    for(int i = 0; i < ROWS; i++){
        double sum = 0;
        #pragma omp parallel for reduction (+:sum) num_threads(NUM_THREADS)
        for (int j = 0; j < COLS; j++) {
            int index = (i*COLS) + j;
            sum += (array[index] - test_array[j]) * (array[index]  - test_array[j]);
        }
        distances[i] = sqrt(sum);
    }

}

void readData(int * array, int * classes){
    ifstream input("mnist_train.csv");

    string foo;
    getline( input, foo );

//    cout << ARRAY_SIZE << endl;

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


int main() {


    auto * array = new int[ARRAY_SIZE];
    auto * normalizedArray = new double[ARRAY_SIZE];
    auto * standardizedArray = new double[ARRAY_SIZE];
    auto * classes = new int[ROWS];

    readData(array, classes);

    double start_time = omp_get_wtime();
    standarize(array, normalizedArray);
    double time = omp_get_wtime() - start_time;
    std::cout << time << "\n";



//    print_arr(normalizedArray);

//    print_arr(array);

//    long a = 7;
//    double start_time = omp_get_wtime();
//    #pragma omp parallel for private(a) num_threads(4) collapse(2)
//    for(int i=0;i<100000;i++) {
//        for(int j=0;j<100000;j++) {
//            a++;
//        }
//    }
//
//    double time = omp_get_wtime() - start_time;
//
//    std::cout << time << "\n";


    return 0;
}