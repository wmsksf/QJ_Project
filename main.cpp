#include <iostream>
#include <climits>
#include <cstring>
#include "project_jj_lib/Utils.h"
#include "project_jj_lib_part2/Utils_part2.h"
#include "project_jj_lib_part2/MACROS.h"

#include <iostream>
#include "project_jj_lib_part2/Utils_part2.h"
#include "project_jj_lib_part3/JobScheduler.h"

//using namespace std;
//
int main(int argc, char **argv)
{
    clock_t start,end;
    start = clock();
    if (argc < 2) {
        std::cout << "give #threads\n";
        exit(1);
    }

    job_scheduler.init(atoi(argv[1]));

    Get_Input();
    Set_output();
    end = clock();

    std::cout << "Total user time: " << ((double) end-start)/CLOCKS_PER_SEC << "s" << std::endl;
    return 0;
}