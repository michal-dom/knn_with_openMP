#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;


void readData(double * array, const string fileName){
    ifstream input(fileName);

    int j = 0;
    for(string line; getline( input, line ); ){
        stringstream stream(line.substr(0, line.size() - 1));
        string digit;
        while( getline(stream, digit, ',') ) {

            array[j] = strtod(digit.c_str(), nullptr);
            j++;
        }
    }
}


void print_arr(double * array){
    long k = 0;
    for(int i = 0; i < 60000; i++){
        for(int j = 0; j < 784; j++){
            cout << array[k] << " ";
            k++;
        }
        cout << endl;
    }
}


void readClasses(int * array, const string fileName){
    ifstream input(fileName);

    int j = 0;
    for(string line; getline( input, line ); ){
        array[j] = strtol(line.c_str(), nullptr, 0);
        j++;

    }
}

void readUnprocessed(int * array, int * classes){
    ifstream input("mnist_train.csv");

    string foo;
    getline( input, foo );

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