//
// Created by wmsksf on 16/10/19.
//


#include "DataTypes.h"

Tuple::Tuple(uint64_t key, uint64_t payload) : key(key), payload(payload) {}

uint64_t Tuple::getKey() const {
    return key;
}

uint64_t Tuple::getPayload() const {
    return payload;
}

Matrix::Matrix(long unsigned int numberOfRows,long unsigned int numberOfColumns)
        :numOfRows(numberOfRows), numOfColumns(numberOfColumns) {
    data= new uint64_t[numberOfColumns*numberOfRows];       //Allocating memory for the matrix data
}

Matrix::~Matrix() {
    delete[] data;
}

bool Matrix::setMatrix(const char* fileName) {

    if(fileName == NULL)
        return false;

    //Opening the input file
    FILE* fp;
    fp = fopen(fileName,"r");

    if (fp == NULL) {   //checking for error with fopen
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    uint64_t number;
    char c;
    long int row = 0;
    long int column = 0;
    int counter = 0;
    while(fscanf(fp,"%llu%c",&number,&c)==2){
        data[row + column*numOfRows] = number;
        column++;
        if(column==numOfColumns){
            row++;
            column = 0;
        }
    }

    fclose(fp);
    return true;
}

void Matrix::printMatrix() {
    long int row = 0;
    long int column = 0;
    int counter = 0;
    while (column!= numOfColumns){
        std::cout << data[row + column*numOfRows] << "|";
        row++;
        counter ++;
        if(counter == numOfColumns) {
            std::cout << std::endl;
            counter = 0;
        }
        if(row == numOfRows){
            row = 0;
            column++;
        }
    }
}

Relation::Relation(Tuple * tuple, int size) :tuples(tuple), num_tuples(size) {
}