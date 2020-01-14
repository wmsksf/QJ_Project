#include <iostream>
#include <climits>
#include <cstring>
#include "project_jj_lib/Utils.h"
#include "project_jj_lib_part2/Utils_part2.h"
#include "project_jj_lib_part2/MACROS.h"

#include <iostream>
#include "project_jj_lib_part2/Utils_part2.h"

//using namespace std;
//
int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cout << "give #threads\n";
        exit(1);
    }
    Get_Input();
    Set_output(atoi(argv[1]));

    return 0;
}