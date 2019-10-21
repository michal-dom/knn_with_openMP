
#include <math.h>
#include <omp.h>

#define NUM_THREADS 3
#define ROWS 60000
#define COLS 784

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