//
// Created by wmsksf on 16/10/19.
//


#include <iostream>
#include <sys/mman.h>
#include <fstream>
#include "DataTypes.h"
#include "../project_jj_lib_part2/MACROS.h"

Tuple::Tuple() { key = 0; }

//void Tuple::setKey(uint64_t key_) { key = key_; }
//uint64_t Tuple::getKey() { return key; }

void Tuple::setPayload(uint64_t payload_) { payloads.push_back(payload_); }
Vector& Tuple::getPayloads() { return payloads; }

void Tuple::swap(Tuple *tpl)
{
//    std::cout << "before swap:" << this->key << " payloads: "; this->payloads.print();
//    std::cout << "next " << tpl->getKey() << " payloads:"; tpl->payloads.print();
//    std::cout << std::endl;

    uint64_t tmp = key;
    this->key = tpl->key;
    tpl->key = tmp;

    Vector tmpayl;
    for (uint64_t i = 0; i < payloads.size(); i++)
        tmpayl.push_back(payloads[i]);

    payloads.clear();
    for (uint64_t i = 0; i < tpl->payloads.size(); i++)
        payloads.push_back(tpl->payloads[i]);

    tpl->payloads.clear();
    for (uint64_t i = 0; i < tmpayl.size(); i++)
        tpl->payloads.push_back(tmpayl[i]);

//    std::cout << "after swap:" << this->key << " payloads: "; this->payloads.print();
//    std::cout << "next " << tpl->getKey() << " payloads:"; tpl->payloads.print();
//    std::cout << std::endl;
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
    if (!numTuples)
    {
        std::cerr << "Empty Relation object." << std::endl
                  << "Tip: call setNumTuples() of object first!" << std::endl;

        exit(EXIT_FAILURE);
    }

    tuples = new Tuple[numTuples];
}

Tuple* Relation::getTuples() const { return tuples; }

//    use in method Matrix::getRelation
void Relation::setTupleVal(long unsigned int index, uint64_t key, uint64_t payload) {
    if(index >= numTuples){
        std::cerr << "Index out of boundaries." << std::endl
                  << "In this Relation object the index can get values from 0 to "
                  << numTuples-1 << std::endl;
        exit(EXIT_FAILURE);
    }
    tuples[index].key = key;
    tuples[index].setPayload(payload);
}

void Relation::setTupleVal(long unsigned int index, uint64_t key, Vector &payload) {
    if(index >= numTuples){
        std::cerr << "Index out of boundaries." << std::endl
                  << "In this Relation object the index can get values from 0 to "
                  << numTuples-1 << std::endl;
        exit(EXIT_FAILURE);
    }

    tuples[index].key = key;
    uint64_t p = payload.size();
    for (uint64_t j = 0; j < p; j++)
        tuples[index].setPayload(payload[j]);
}

void Relation::clean()
{
    for (uint64_t i = 0; i < numTuples; i++)
    {
        tuples[i].key = 0;
        tuples[i].getPayloads().clear();
    }
}
void Relation::print() {
    if (!numTuples) {
        log("Empty Relation object\n");
        return;
    }
    uint64_t j = numTuples;
    Tuple *t = tuples;
    for (uint64_t i = 0; i < j; i++)
        t[i].print();
}

bool Relation::isSorted() {
    uint64_t  size = numTuples;
    if(!size){
        log("Relation is empty\n");
        return false;
    }
    uint64_t a = tuples[0].key;
    for (uint64_t i =1; i<size; i++){
        uint64_t b = tuples[i].key;
        if(a > b){
            log("Relation is not sorted\n");
            return false;
        }
        a = b;
    }
//    std::cout << "Relation is sorted" << std::endl;
    return true;
}

void Relation::copyTuplesVal(Relation *R, uint64_t start, uint64_t end) {
    if (!numTuples)
    {
        std::cerr << "Empty Relation object." << std::endl
                  << "Tip: call setNumTuples() of object first!" << std::endl;
        exit(EXIT_FAILURE);
    }
    else if (R->numTuples != numTuples)
    {
        std::cerr << "Relation objects of different size." << std::endl;
        exit(EXIT_FAILURE);
    }
    else if(numTuples <= end or numTuples <= start)
    {
        std::cerr << "Start or end value is out of bounds" << std::endl;
        exit(EXIT_FAILURE);
    }

    for (uint64_t i = start; i <= end; i++)
    {
        tuples[i].key = R->tuples[i].key;
        uint64_t p = R->tuples[i].payloads.size();
        for (uint64_t j = 0; j < p; j++)
            tuples[i].setPayload(R->getTuples()[i].payloads[j]);
    }
}

void Relation::filter(Vector  *vector) {
    if (vector == nullptr)
        return;

    uint64_t  vectorSize = vector->size();
    if(vectorSize == 0){
        numTuples = 0;
        delete[] tuples;
        tuples = nullptr;
    }
    if(vectorSize == numTuples) return;

    Tuple* tmp = new Tuple[vectorSize];

    for(int i =0; i<vectorSize;i++)
    {
        uint64_t row = (*vector)[i];
        tmp[i].key = tuples[row].key;

        for (uint64_t j =0; j < tuples[row].payloads.size(); j++)
            tmp[i].setPayload(tuples[row].payloads[j]);
    }

    numTuples = vectorSize;
    delete[] tuples;
    tuples = tmp;
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
        exit(EXIT_FAILURE);
    }

    for (int j = 0; j < numOfColumns*numOfRows; j++)
        infile.read((char*)(data+j), sizeof(uint64_t));

    return true;
}

Relation *Matrix::getRelation(long unsigned int columnNumber) {
    if(columnNumber >=numOfColumns)
    {
        log("Out of Matrix object data array bounds\n");
        return nullptr;
    }

    long unsigned int offset = columnNumber*numOfRows;
    auto* R = new Relation();
    R->numTuples = numOfRows;
    R->initTuples();
    for(uint64_t i = 0; i<numOfRows; i++){
        R->setTupleVal(i,data[offset+i],i);
    }
    return R;
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

uint64_t *Matrix::getData() { return data; }

Relation *Matrix::getRelation(List* list,int index, long int numOfRows_, int columnNumber) {
    if(list == nullptr or list->getHead() == nullptr)
    {
        log("No List object\n");
        return nullptr;
    }
    if(numOfRows_ == 0)
    {
        log("No rows\n");
        return nullptr;
    }
    if(index >= list->getHead()->data.size())
    {
        log("Index of Vector class vec array out of bounds in struct Node\n");
        return nullptr;
    }

    if(columnNumber >=numOfColumns)
    {
        log("Out of Matrix object data array bounds\n");
        return nullptr;
    }

    long unsigned int offset = columnNumber*numOfRows;
    auto* R = new Relation();
    R->numTuples = numOfRows_;
    R->initTuples();
    long int i = 0;
    for(struct Node* N = list->getHead(); N != nullptr; N = N->next){
        R->setTupleVal(i,data[offset+N->data[index]],N->data);
        i++;
    }
    return R;

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
    Buffer[index].key = x;
    Buffer[index].setPayload(y);

    index++;
}
bool Results::isFull() { return (index == BUFFERSIZE-1); }
bool Results::isEmpty() { return (index == 0); }

void Results::print()
{
    for (uint64_t i = 0; i < index; i++) {
        std::cout << Buffer[i].key << "\t";
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
