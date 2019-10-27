//
// Created by michal on 21.10.19.
//

#ifndef PRIR_NORMSCALER_H
#define PRIR_NORMSCALER_H

int min(const int col, const int * array);

int max(const int col, const int * array);

void minMaxNormalization(const int col, const int * array, double * normalizedArray);

#endif //PRIR_NORMSCALER_H
