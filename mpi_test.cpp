//#include <iostream>
//#include <stdio.h>
////#include <mpi.h>
//#include <random>
//#include <float.h>
//#include <math.h>
//
//#define NUM_PROCS 4
//
//double * createNums(int num_elements) {
//    auto *nums = new double[num_elements];
//
//    for (int i = 0; i < num_elements; i++) {
//        nums[i] = (double) i;
//    }
//    return nums;
//}
//
//double mean(double * nums, int len){
//
//    int num_elements_per_proc = len/NUM_PROCS;
//
//
//    MPI_Init(NULL, NULL);
//
//    int world_rank;
//    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
//    int world_size;
//    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
//
//    double local_sum = 0;
//    for (int i = 0; i < num_elements_per_proc; i++) {
//        local_sum += nums[i + (num_elements_per_proc * world_rank)];
//    }
//
//    double global_sum;
//    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0,
//               MPI_COMM_WORLD);
//
//    MPI_Barrier(MPI_COMM_WORLD);
////    MPI_Finalize();
//
////    free(nums);
//
//    return global_sum/len;
//}
//
//
//double min(double * nums, int len){
//    int num_elements_per_proc = len/NUM_PROCS;
//
//
//    MPI_Init(NULL, NULL);
//
//    int world_rank;
//    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
//    int world_size;
//    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
//
//    auto local_min = DBL_MAX;
//    for (int i = 0; i < num_elements_per_proc; i++) {
//        int index = i + (num_elements_per_proc * world_rank);
//        if(local_min > nums[index]){
//            local_min = nums[index];
//        }
//
//    }
//
//    double global_min;
//    MPI_Reduce(&local_min, &global_min, 1, MPI_DOUBLE, MPI_MIN, 0,
//               MPI_COMM_WORLD);
//
//    MPI_Barrier(MPI_COMM_WORLD);
//    MPI_Finalize();
//
//    free(nums);
//
//    return global_min;
//
//}
//
//double max(double * nums, int len){
//    int num_elements_per_proc = len/NUM_PROCS;
//
//
//    MPI_Init(NULL, NULL);
//
//    int world_rank;
//    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
//    int world_size;
//    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
//
//    auto local_max = 0.0;
//    for (int i = 0; i < num_elements_per_proc; i++) {
//        int index = i + (num_elements_per_proc * world_rank);
//        if(local_max < nums[index]){
//            local_max = nums[index];
//        }
//
//    }
//
//    double global_max;
//    MPI_Reduce(&local_max, &global_max, 1, MPI_DOUBLE, MPI_MAX, 0,
//               MPI_COMM_WORLD);
//
//    MPI_Barrier(MPI_COMM_WORLD);
//    MPI_Finalize();
//
//    free(nums);
//
//    return global_max;
//
//}
//
//
//double deviation(double * nums, int len){
//    int num_elements_per_proc = len/NUM_PROCS;
//    double meanVal = mean(nums, len);
//
////    MPI_Init(NULL, NULL);
//
//    int world_rank;
//    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
//    int world_size;
//    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
//
//    double local_sum = 0;
//    for (int i = 0; i < num_elements_per_proc; i++) {
//        int index = i + (num_elements_per_proc * world_rank);
//
//        local_sum += (nums[index] - meanVal) * (nums[index] - meanVal);
//    }
//
//    double global_sum;
//    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0,
//               MPI_COMM_WORLD);
//
//    MPI_Barrier(MPI_COMM_WORLD);
//    MPI_Finalize();
//
//    double variance = global_sum / len;
//    return sqrt(variance);
//
//
//}
//
//
//int main (int argc, char *argv[])
//{
////    int rank, comm_size;
////
//    int array_len = 1000;
////
////    int num_elements_per_proc = array_len/4;
////
//    auto * nums = createNums(array_len);
////
////    MPI_Init(NULL, NULL);
////
////    int world_rank;
////    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
////    int world_size;
////    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
////
////
////
////    float local_sum = 0;
////    for (int i = 0; i < num_elements_per_proc; i++) {
////        local_sum += numbs[i + (num_elements_per_proc * world_rank)];
////    }
////
////    printf("Local sum for process %d - %f, avg = %f\n",
////           world_rank, local_sum, local_sum / num_elements_per_proc);
////
////    float global_sum;
////    MPI_Reduce(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM, 0,
////               MPI_COMM_WORLD);
//////
////    if (world_rank == 0) {
////        printf("Total sum = %f, avg = %f\n", global_sum,
////               global_sum / (world_size * num_elements_per_proc));
////    }
////    std::cout << mean(nums, array_len) << std::endl;
//    auto val = deviation(nums, array_len);
//    std::cout << val << std::endl;
//
////
//
////    free(numbs);
////
////
////    MPI_Barrier(MPI_COMM_WORLD);
////    MPI_Finalize();
//}