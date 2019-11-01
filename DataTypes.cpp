//
// Created by wmsksf on 16/10/19.
//


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

void Tuple::print() {
    std::cout << this->getKey() << "  " << this->getPayload() << std::endl;
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
        std::cout << "Relation objects of different size." << std::endl;
        exit(EXIT_FAILURE);
    }

    tuples = new Tuple[this->getNumTuples()];
    for (uint64_t i = 0; i < this->getNumTuples(); i++)
    {
        tuples[i].setKey(R->tuples[i].getKey());
        tuples[i].setPayload(R->tuples[i].getPayload());
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

void Relation::setTupleVal(long unsigned int index, uint64_t key, uint64_t payload) {
    if(index >= this->getNumTuples()){
        std::cerr << "Index out of boundaries." << std::endl
                  << "In this Relation object the index can get values from 0 to "
                  << this->getNumTuples() << std::endl;
        exit(EXIT_FAILURE);
    }
    this->getTuples()[index].setKey(key);
    this->getTuples()[index].setPayload(payload);
}

void Relation::print() {
    if (!this->getNumTuples()) {
        std::cout << "Empty Relation object." << std::endl;
        return;
    }
    uint64_t j = this->getNumTuples();
    Tuple *t = this->getTuples();
    std::cout << j << std::endl;
    for (uint64_t i = 0; i < j; i++){
        std::cout << i << ": ";
        t[i].print();
    }
}

bool Relation::isSorted() {
    Tuple* tuples_ = this->getTuples();
    uint64_t  size = this->getNumTuples();
    if(size ==0){
        std::cout << "Relation is empty" << std::endl;
        return false;
    }
    uint64_t a = tuples_[0].getKey();
    for (uint64_t i =1; i<size; i++){
        uint64_t b = tuples_[i].getKey();
        if(a > b){
            std::cout << "Relation is not sorted" << std::endl;
            return false;
        }
        a = b;
    }
    std::cout << "Relation is sorted" << std::endl;
    return true;
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
//       !!
        std::cout << data[row + column*numOfRows] << ",";
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

Relation *Matrix::getRelation(long unsigned int columnNumber) {
    if(columnNumber >=numOfColumns) {
        std::cout << "Out of matrix boundaries. The matrix has only "<< numOfColumns <<
                  " columns.  You tries to access column " << columnNumber << std::endl;
        return nullptr;
    }

    long unsigned int offset = columnNumber*numOfRows;
    auto* R = new Relation();
    R->setNumTuples(this->numOfRows);
    R->initTuples();
    for(uint64_t i = 0; i<numOfRows; i++){
        R->setTupleVal(i,data[offset+i],i);
    }
    return R;
}

Results::Results() { index = 0; }

void Results::add(uint64_t x, uint64_t y)
{
    Buffer[index].setKey(x);
    Buffer[index].setPayload(y);

    index++;
}

bool Results::isFull() { return (index == BUFFERSIZE-1); }
bool Results::isEmpty() { return (index == 0); }

void Results::print()
{
    for (uint64_t i = 0; i < index; i++)
        std::cout << Buffer[i].getKey() << "\t" << Buffer[i].getPayload() << std::endl;

    std::cout << std::endl;
}
