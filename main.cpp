#include <iostream>
#include "Utils.h"

using namespace std;

int main(int argc, char **argv)
{

    char* file = "r0.tbl";
    Tuple a = getMatrixSize(file);
    cout << a.getKey() << " " << a.getPayload() << endl;
    Matrix* matrix = new Matrix(a.getPayload(),a.getKey());
    if(matrix->setMatrix(file))
        cout << "success" << endl;
    matrix->printMatrix();
    return 0;
}
