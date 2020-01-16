#include <iostream>
#include <climits>
#include <cstring>
#include "project_jj_lib/Utils.h"
#include "project_jj_lib_part2/Utils_part2.h"
#include "project_jj_lib_part2/MACROS.h"

#include <iostream>
#include "project_jj_lib_part2/Utils_part2.h"
#include "project_jj_lib_part3/JobScheduler.h"

JobScheduler job_scheduler;

//using namespace std;
//
int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cout << "give #threads\n";
        exit(1);
    }

    job_scheduler.init(atoi(argv[1]));

    Get_Input();
    Set_output();

    return 0;
}