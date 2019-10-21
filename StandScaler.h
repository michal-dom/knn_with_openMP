//
// Created by michal on 21.10.19.
//

#ifndef PRIR_STANDSCALER_H
#define PRIR_STANDSCALER_H

double mean(const int col, const int * array);

double deviation(int col, const int * array);

void standardization(int col, const int * array, double * standardizedArray);


#endif //PRIR_STANDSCALER_H
