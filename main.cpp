#include <iostream>
#include "Utils.h"

using namespace std;

int main(int argc, char **argv)
{
    const char* file1 = "relA";
    Tuple a = getMatrixSize(file1);
    Matrix* matrix1 = new Matrix(a.getPayload(),a.getKey());
    if(matrix1->setMatrix(file1))
        cout << "success" << endl;

    const char* file2 = "relB";
    Tuple b = getMatrixSize(file2);
    Matrix* matrix2 = new Matrix(b.getPayload(),b.getKey());
    if(matrix2->setMatrix(file2))
        cout << "success" << endl;

    Relation* R1,*R2;
//    R1 = matrix1->getRelation(0);
//    R2 = matrix2->getRelation(0);
    R1 = matrix1->getRelation(0);
    R2 = matrix1->getRelation(1);

    LinkedList *ResultsList = SortMergeJoin(R1,R2);
    if (!ResultsList)
        std::cout << "Sorting failed!" << std::endl;
    else
        if (ResultsList->empty())
            std::cout << "No results!" << std::endl;
        else
        {
            std::cout << "RowIdA\tRowIdB" << std::endl;
            ResultsList->print();
            delete ResultsList;
        }

    return 0;
}
