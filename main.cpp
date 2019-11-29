#include <iostream>
#include <climits>
#include <cstring>
#include "project_jj_lib/Utils.h"
#include "project_jj_lib_part2/Utils_part2.h"
#include "project_jj_lib_part2/MACROS.h"

using namespace std;

int main(int argc, char **argv)
{
    Get_Relations();
    std::cout << "total matrices: " << MATRICES_SIZE << std::endl;
//    for (int i = 0; i < MATRICES_SIZE; i++)
//    {
//        std::cout << i << " matrix" << std::endl;
//        MATRICES[i].printMatrix();
//        std::cout << std::endl << std::endl;
//    }

//    char* input = new char[100];
//    strcpy(input,"3 0 1|0.2=1.0&0.1=2.0&0.2>3499|1.2 0.1");
//
//    Query q;
//    q.parse(input);
//    std::cout << "matrices: ";
//    for(int i = 0; i < q.NumOfMatrices; i++)   std::cout << q.Matrices[i] << " ";
//    std::cout << "\nresults: ";
//    for (int i = 0; i < q.NumOfResults; i++)   std::cout << q.Results[i] << " ";
//    std::cout << "\npredicates: " << q.NumOfPredicates << std::endl;
//    for (int i = 0; i < q.NumOfPredicates; i++)
//    {
//        std::cout << std::endl;
//        std::cout << "operator: " << q.Predicates[i].operation << std::endl;
//        std::cout << "filter: " << q.Predicates[i].filter << std::endl;
//        std::cout << "matrices[0]: " << q.Predicates[i].Matrices[0] << std::endl;
//        std::cout << "matrices[1]: " << q.Predicates[i].Matrices[1] << std::endl;
//
//        std::cout << "rowids[0]: " << q.Predicates[i].RowIds[0] << std::endl;
//        std::cout << "rowids[1]: " << q.Predicates[i].RowIds[1] << std::endl;
//    }
//
//    q.exec();

    Relation *R1,*R2;
    R1 = MATRICES[0].getRelation(1);
    R2 = MATRICES[1].getRelation(1);
    SortMergeJoin(R1,R2);

    return 0;
}
