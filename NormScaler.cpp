#define NUM_PROCS 3
#define ROWS 60000
#define COLS 784
//#include <mpi.h>


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
//
//
//void normalize(const int * array, double * normalizedArray){
//
//    int colsNumberPerProcess = COLS/NUM_PROCS;
//
////    MPI_Init(NULL, NULL);
//////
////    int world_rank;
////    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
////    int world_size;
////    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
////
////    for(int i = 0; i < colsNumberPerProcess; i++){
////        int c = i + (colsNumberPerProcess * world_rank);
////        minMaxNormalization(i, array, normalizedArray);
////    }
////
////    MPI_Barrier(MPI_COMM_WORLD);
////    MPI_Finalize();
//
//
//
//
//
//}


//void minMaxNormalizationMPI(const int col, const int * array, double * normalizedArray){

//    int num_elements_per_proc = ROWS/NUM_PROCS;
//
//    MPI_Init(NULL, NULL);
//
//    int world_rank;
//    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
//    int world_size;
//    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
//
//
//    // min
//    auto local_min = 255;
//    for (int i = 0; i < num_elements_per_proc; i++) {
//        int index = (i*COLS) + col + (num_elements_per_proc * world_rank);
//        if(local_min > array[index]){
//            local_min = array[index];
//        }
//    }
//
//
//    double global_min;
//    MPI_Reduce(&local_min, &global_min, 1, MPI_DOUBLE, MPI_MIN, 0,
//               MPI_COMM_WORLD);
//
//
//    MPI_Barrier(MPI_COMM_WORLD);
//    // max
//
//    auto local_val = 0;
//
//    for (int i = 0; i < num_elements_per_proc; i++) {
//        int index = (i*COLS) + col + (num_elements_per_proc * world_rank);
//        if (local_val > array[index]){
//            local_val = array[index];
//        }
//    }
//
//    double global_max;
//    MPI_Reduce(&local_max, &global_max, 1, MPI_DOUBLE, MPI_MAX, 0,
//               MPI_COMM_WORLD);
//
//    MPI_Barrier(MPI_COMM_WORLD);
//
//
//
//
//    const int min_val = min(col, array);
//    const int max_val = max(col, array);
//    const double denominator = max_val - min_val;
//
//    for(int i = 0; i < ROWS; i++){
//        int index = (i*COLS) + col;
//        if(denominator == 0){
//            normalizedArray[index] = 0;
//        } else {
//            normalizedArray[index] = (array[index] - min_val) / denominator;
//        }
//    }
//}