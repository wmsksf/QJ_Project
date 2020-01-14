//
// Created by wmsksf on 22/11/19.
//

#ifndef PROJECT_JJ_UTILS_PART2_H
#define PROJECT_JJ_UTILS_PART2_H

#include <cstdint>
#include "../project_jj_lib/DataTypes.h"
#include "../project_jj_lib_part2/Datatypes_part2.h"

#define MAX_DATASETS 128

char** Get_Input(bool flag = false, uint64_t *size = nullptr);
void Set_output(int threads);

#endif //PROJECT_JJ_UTILS_PART2_H
