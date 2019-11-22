#include <iostream>
#include <climits>
#include <cstring>
#include "project_jj_lib/Utils.h"
#include "project_jj_lib_part2/Utils_part2.h"

using namespace std;

int main(int argc, char **argv)
{
    uint64_t matrices_size = 0;
    Matrix *matrices = Get_Relations(matrices_size);
    std::cout << "total matrices: " << matrices_size << std::endl;

    return 0;
}
