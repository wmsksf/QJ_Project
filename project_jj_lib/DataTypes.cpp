//
// Created by wmsksf on 16/10/19.
//


#include <iostream>
#include <sys/mman.h>
#include <fstream>
#include "DataTypes.h"
#include "../project_jj_lib_part2/MACROS.h"

Tuple::Tuple() { key = 0; }

void Tuple::setKey(uint64_t key_) { key = key_; }
uint64_t Tuple::getKey() { return key; }

void Tuple::setPayload(uint64_t payload_) { payloads.push_back(payload_); }
Vector& Tuple::getPayloads() { return payloads; }

void Tuple::swap(Tuple *tpl)
{
    uint64_t tmp = key;
    setKey(tpl->getKey());
    tpl->setKey(tmp);

    Vector tmpayl;
    tmpayl = payloads;
    payloads = tpl->getPayloads();
    tpl->getPayloads() = tmpayl;
}

void Tuple::print()
{
    std::cout << "key: " << key << " payloads: ";
    payloads.print();
    std::cout << std::endl;
}
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
    ALLOC_CHECK(tuples);
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
    ALLOC_CHECK(tuples);

    for (uint64_t i = 0; i < this->getNumTuples(); i++)
    {
        tuples[i].setKey(R->tuples[i].getKey());
        tuples[i].getPayloads() = R->tuples[i].getPayloads();
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

void Relation::setTupleVal(long unsigned int index, uint64_t key, Vector &payload) {
    if(index >= this->getNumTuples()){
        std::cerr << "Index out of boundaries." << std::endl
                  << "In this Relation object the index can get values from 0 to "
                  << this->getNumTuples()-1 << std::endl;
        exit(EXIT_FAILURE);
    }
    this->getTuples()[index].setKey(key);
    this->getTuples()[index].payloads = payload;
}

void Relation::clean()
{
    for (uint64_t i = 0; i < numTuples; i++)
    {
        getTuples()[i].setKey(0);
        getTuples()[i].getPayloads().clear();
    }
}
void Relation::print() {
    if (!this->getNumTuples()) {
        std::cout << "Empty Relation object." << std::endl;
        return;
    }
    uint64_t j = this->getNumTuples();
    Tuple *t = this->getTuples();
    for (uint64_t i = 0; i < j; i++)
        t[i].print();
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
        tuples[i].payloads = R->tuples[i].getPayloads();
    }
}

void Relation::filter(Vector * vector) {
    if (vector == nullptr) {
        return;
        std::cout << "empty" << std::endl;
    }

    uint64_t  vectorSize = vector->size();
    if(vectorSize == 0){
        numTuples = 0;
        delete[] tuples;
        tuples = nullptr;
    }
    if(vectorSize == numTuples) return;
    Tuple* tmp = new Tuple[vectorSize];
    ALLOC_CHECK(tmp);

    std::cout << "in filter of relation object" << std::endl;
    for(int i =0; i<vectorSize;i++){
        uint64_t row = (*vector)[i];
        tmp[i].setKey(tuples[row].getKey());
        tmp[i].getPayloads() = tuples[row].getPayloads();

        std::cout << "key " << tmp[i].getKey() << " payloads "; tmp[i].getPayloads().print();std::cout << std::endl;

    }
    numTuples = vectorSize;
    delete[] tuples;
    tuples = tmp;
    for (uint64_t i = 0; i < numTuples;i++) tmp[i].print();

    std::cout << "PRINT AT END of filter of relation object" << std::endl;
    for (uint64_t i = 0; i < numTuples;i++) tuples[i].print();
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

long unsigned int Matrix::getNumOfRows() {
    return numOfRows;
}

long unsigned int Matrix::getNumOfColumns() {
    return numOfColumns;
}

uint64_t *Matrix::getData() {
    return data;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Results::Results() { Buffer = nullptr; Buffersize = 0; }
Results::~Results() { delete[] Buffer; }

void Results::setBuffersize(uint64_t buffersize) { Buffersize = buffersize; }

void Results::initBuffer()
{
    if (!Buffersize)
    {
        std::cout << "Empty Results object." << std::endl <<
        "Tip: call setBuffersize() of object first!" << std::endl;

        exit(EXIT_FAILURE);
    }

    Buffer = new Tuple[Buffersize];
}

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
    for (uint64_t i = 0; i < index; i++) {
        std::cout << Buffer[i].getKey() << "\t";
        Buffer[i].getPayloads().print();
    }

    std::cout << std::endl;
}

Tuple *Results::getBuffer() {
    return Buffer;
}

uint64_t Results::getIndex() {
    return index;
}
