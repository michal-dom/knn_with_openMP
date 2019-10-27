#define NUM_THREADS 3
#define ROWS 60000
#define COLS 784
#include <omp.h>


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