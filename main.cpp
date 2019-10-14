#include <iostream>
#include <fstream>
#include <sstream>
#include <omp.h>
#include <stdio.h>
#include <math.h>

using namespace std;

const int ROWS = 60000;
const int COLS = 784;
const long ARRAY_SIZE = ROWS * COLS;


int min(int col, const int * array){
    int min_val = 255;
    #pragma omp parallel for reduction(min: min_val) num_threads(4)
    for(int i = 0; i < ROWS; i++){
        int index = (i*784) + col;
        if (min_val > array[index]){
            min_val = array[index];
        }
    }

    return min_val;
}

int max(int col, const int * array){
    int max_val = 0;
    #pragma omp parallel for reduction(max: max_val) num_threads(4)
    for(int i = 0; i < ROWS; i++){
        int index = (i*784) + col;
        if (max_val < array[index]){
            max_val = array[index];
        }
    }

    return max_val;
}

double * minMaxNormalization(int col, const int * array){

    const int min_val = min(col, array);
    const int max_val = max(col, array);
    const double denominator = max_val - min_val;

    auto * normalizedArray = new double[ARRAY_SIZE];

    #pragma omp parallel for num_threads(4)
    for(int i = 0; i < ROWS; i++){
        int index = (i*784) + col;
        normalizedArray[index] = (array[index] - min_val) / denominator;
    }

    return normalizedArray;
}

double mean(int col, const int * array){
    double sum = 0;

    #pragma omp parallel for reduction (+:sum) num_threads(4)
    for(int i = 0; i < ROWS; i++){
        int index = (i*784) + col;
        sum += array[index];
    }

    return sum / ROWS;
}

double deviation(int col, const int * array){
    double sum = 0;
    const double mean_val = mean(col, array);

    #pragma omp parallel for reduction (+:sum) num_threads(4)
    for(int i = 0; i < ROWS; i++){
        int index = (i*784) + col;
        sum += (array[index] - mean_val) * (array[index] - mean_val);
    }
    double variance = sum / ROWS;
    return sqrt(variance);
}

double * standardization(int col, const int * array){
    const double mean_val = mean(col, array);
    const double deviation_val = deviation(col, array);

    auto * standardizedArray = new double[ARRAY_SIZE];

    #pragma omp parallel for num_threads(4)
    for(int i = 0; i < ROWS; i++){
        int index = (i*784) + col;
        standardizedArray[index] = (array[index] - mean_val) / deviation_val;
    }

    return standardizedArray;
}


void print_col(int col, int * array){
    for(int i = 0; i < ROWS; i++){
        int index = (i*784) + col;
        if(array[index] > 0){
            cout << array[index] << endl;
        }

    }
}


void euclidian(const double * array, const double * new_array){
    double sum = 0;
    auto * votes = new double[10];

    #pragma omp parallel for reduction (+:sum) collapse(2) num_threads(4)
    for(int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; ++j) {
            int index = (i*784) + j;
            sum += (array[index] - new_array[j]) * (array[index]  - new_array[j]);
        }
    }

}

void readData(int * array, int * classes){
    ifstream input("mnist_train.csv");

    string foo;
    getline( input, foo );

    cout << ARRAY_SIZE << endl;

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


int main() {


    auto * array = new int[ARRAY_SIZE];
    auto * classes = new int[ROWS];

    readData(array, classes);


    double start_time = omp_get_wtime();

    cout << deviation(60, array) << endl;

    double time = omp_get_wtime() - start_time;

    std::cout << time << "\n";

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