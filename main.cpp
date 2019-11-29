#include <iostream>
#include <climits>
#include <cstring>
#include "project_jj_lib/Utils.h"
#include "project_jj_lib_part2/Utils_part2.h"
#include "project_jj_lib_part2/MACROS.h"

using namespace std;

int main(int argc, char **argv)
{
//    get relations
    Get_Input();
    std::cout << "total matrices: " << MATRICES_SIZE << std::endl;
//    for (int i = 0; i < MATRICES_SIZE; i++)
//    {
//        std::cout << i << " matrix" << std::endl;
//        MATRICES[i].printMatrix();
//        std::cout << std::endl << std::endl;
//    }

    Set_output();

//    char* input = new char[100];
//    strcpy(input,"3 0 1|0.2=1.0&0.1=2.0&0.2>3499|1.2 0.1");
//    Query q;
//    q.parse(input);
//    q.exec();
//
//    Relation *R1,*R2;
//    R1 = MATRICES[0].getRelation(1);
//    R2 = MATRICES[1].getRelation(1);
//
//    LinkedList* rslt = SortMergeJoin(R1,R2);
////    rslt->print();

    return 0;
}
