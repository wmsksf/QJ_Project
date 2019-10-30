#include <iostream>
#include "Utils.h"

using namespace std;

int main(int argc, char **argv)
{


    char* file = "r3.tbl";
    Tuple a = getMatrixSize(file);
    cout << a.getKey() << " " << a.getPayload() << endl;
    Matrix* matrix = new Matrix(a.getPayload(),a.getKey());
    if(matrix->setMatrix(file))
        cout << "success" << endl;
    matrix->printMatrix();
    Relation* R;
    R = matrix->getRelation(1);
    cout << "success" << endl;
    R->print();
    Radixsort(R,0,R->getNumTuples()-1,0, nullptr);
    R->print();
    return 0;
}
