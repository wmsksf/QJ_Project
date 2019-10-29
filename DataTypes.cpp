//
// Created by wmsksf on 16/10/19.
//

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "DataTypes.h"

Tuple::Tuple() { key = payload = 0; }

uint64_t Tuple::getKey() const {
    return key;
}

uint64_t Tuple::getPayload() const {
    return payload;
}

void Tuple::setKey(uint64_t key_) {
    key = key_;
}

void Tuple::setPayload(uint64_t payload_) {
    payload = payload_;
}

void Tuple::swap(Tuple* tuple)
{
    uint64_t tmp = this->getKey();
    this->setKey(tuple->getKey());
    tuple->setKey(tmp);

    tmp = this->getPayload();
    this->setPayload(tuple->getPayload());
    tuple->setPayload(tmp);
}

Relation::Relation() { tuples = nullptr; numTuples = 0; }
Relation::~Relation() { delete[](tuples); }

void Relation::initTuples()
{
    if (!this->getNumTuples())
    {
        std::cerr << "Empty Relation object." << std::endl
                  << "Tip: call setNumTuples() of object first!" << std::endl;

        exit(EXIT_FAILURE);
    }

    tuples = new Tuple[this->getNumTuples()];
}

void Relation::initTuplesVal(Relation* R)
{
    if (!this->getNumTuples())
    {
        std::cerr << "Empty Relation object." << std::endl
        << "Tip: call setNumTuples() of object first!" << std::endl;

        exit(EXIT_FAILURE);
    }
    else if (R->getNumTuples() != this->getNumTuples())
    {
        std::cerr << "Relation objects of different size." << std::endl;
        exit(EXIT_FAILURE);
    }

    tuples = new Tuple[this->getNumTuples()];
    for (uint64_t i = 0; i < this->getNumTuples(); i++)
    {
        tuples[i].setKey(R->tuples[i].getKey());
        tuples[i].setPayload(0);
    }

}

Tuple* Relation::getTuples() const {
    return tuples;
}

uint64_t Relation::getNumTuples() const {
    return numTuples;
}

void Relation::setNumTuples(uint64_t numTuples_) {
    numTuples = numTuples_;
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
    while(fscanf(fp,"%lu%c",&number,&c)==2){
        std:: cout << counter++ << "  " << number << std::endl;
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
