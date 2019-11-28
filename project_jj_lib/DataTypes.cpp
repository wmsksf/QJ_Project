//
// Created by wmsksf on 16/10/19.
//


#include <iostream>
#include <sys/mman.h>
#include <fstream>
#include "DataTypes.h"

Tuple::Tuple() { key = payload = 0; }

uint64_t Tuple::getKey() const { return key; }
uint64_t Tuple::getPayload() const { return payload; }

void Tuple::setKey(uint64_t key_) { key = key_; }
void Tuple::setPayload(uint64_t payload_) { payload = payload_; }

void Tuple::swap(Tuple* tuple)
{
    uint64_t tmp = this->getKey();
    this->setKey(tuple->getKey());
    tuple->setKey(tmp);

    tmp = this->getPayload();
    this->setPayload(tuple->getPayload());
    tuple->setPayload(tmp);
}

bool Tuple::equal(Tuple x) { return (this->getKey() == x.getKey() && this->getPayload() == x.getPayload()); }

void Tuple::print() { std::cout << this->getKey() << "  " << this->getPayload() << std::endl; }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Relation::Relation() { tuples = nullptr; numTuples = 0; }
Relation::~Relation() { delete[] tuples; }

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

// copy data from a given relation
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

Tuple* Relation::getTuples() const { return tuples; }
uint64_t Relation::getNumTuples() const { return numTuples; }
void Relation::setNumTuples(uint64_t numTuples_) { numTuples = numTuples_; }

void Relation::setTupleVal(long unsigned int index, uint64_t key, uint64_t payload) {
    if(index >= this->getNumTuples()){
        std::cerr << "Index out of boundaries." << std::endl
                  << "In this Relation object the index can get values from 0 to "
                  << this->getNumTuples()-1 << std::endl;
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

void Relation::copyTuplesVal(Relation *R, uint64_t start, uint64_t end) {
    uint64_t size = this->getNumTuples();
    if (!size)
    {
        std::cerr << "Empty Relation object." << std::endl
                  << "Tip: call setNumTuples() of object first!" << std::endl;

        exit(EXIT_FAILURE);
    }
    else if (R->getNumTuples() != size)
    {
        std::cout << "Relation objects of different size." << std::endl;
        exit(EXIT_FAILURE);
    }
    else if(size <= end or size <= start)
    {
        std::cout << "start or end value is out of index" << std::endl;
        exit(EXIT_FAILURE);
    }

    for (uint64_t i = start; i <= end; i++)
    {
        tuples[i].setKey(R->tuples[i].getKey());
        tuples[i].setPayload(R->tuples[i].getPayload());
    }
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Matrix::Matrix() {}

Matrix::~Matrix()
{ munmap(data, sizeof(uint64_t)*numOfColumns*numOfRows); }

bool Matrix::setMatrix(char* fileName)
{
    std::ifstream infile(fileName, std::ios::binary | std::ios_base::app);

    infile.read((char*) &numOfRows, sizeof(uint64_t));
    infile.read((char*) &numOfColumns, sizeof(uint64_t));

    data = (uint64_t*) mmap(nullptr, sizeof(uint64_t)*numOfColumns*numOfRows, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (data == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    for (int j = 0; j < numOfColumns*numOfRows; j++)
        infile.read((char*)(data+j), sizeof(uint64_t));

    return true;
}

void Matrix::printMatrix() {
    uint64_t row = 0;
    uint64_t column = 0;
    uint64_t counter = 0;
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