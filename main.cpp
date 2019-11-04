#include <iostream>
#include "project_jj_lib/Utils.h"

using namespace std;

int main(int argc, char **argv)
{
    const char* file1 = "relB_tiny";
    Tuple a = getMatrixSize(file1);
    Matrix* matrix1 = new Matrix(a.getPayload(),a.getKey());
    if(!matrix1->setMatrix(file1))
    {
        cout << "Set up of matrix object failed." << endl;
        exit(EXIT_FAILURE);
    }

    const char* file2 = "relA_tiny";
    Tuple b = getMatrixSize(file2);
    Matrix* matrix2 = new Matrix(b.getPayload(),b.getKey());
    if(!matrix2->setMatrix(file2))
    {
        cout << "Set up of matrix object failed." << endl;
        exit(EXIT_FAILURE);
    }

    Relation *R1,*R2;
    R1 = matrix1->getRelation(0);
    R2 = matrix2->getRelation(0);

    LinkedList *ResultsList = SortMergeJoin(R1,R2);

    if (!ResultsList)
        std::cout << "Sorting failed!" << std::endl;
    else
    {
        if (ResultsList->empty())
            std::cout << "No results!" << std::endl;
        else
        {
            std::cout << "RowIdA\tRowIdB" << std::endl;
            ResultsList->print();
        }
    }

    clean_up(&matrix1, &matrix2, &R1, &R2, &ResultsList);



    return 0;
}
