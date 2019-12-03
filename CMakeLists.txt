cmake_minimum_required(VERSION 3.13)
project(project_jj)

set(CMAKE_CXX_STANDARD 11)

add_executable(project_jj main.cpp project_jj_lib/DataTypes.cpp project_jj_lib/DataTypes.h project_jj_lib/Utils.cpp project_jj_lib/Utils.h project_jj_lib/LinkedList.cpp project_jj_lib/LinkedList.h project_jj_lib/Stack.cpp project_jj_lib/Stack.h project_jj_lib_part2/Utils_part2.cpp project_jj_lib_part2/Utils_part2.h project_jj_lib_part2/Datatypes_part2.cpp project_jj_lib_part2/Datatypes_part2.h project_jj_lib_part2/MACROS.h project_jj_lib_part2/Vector.cpp project_jj_lib_part2/Vector.h project_jj_lib_part2/List.cpp project_jj_lib_part2/List.h)

#   >>>>>>>>>>>>>>>>
#   in order to BUILD executable for testing in project
#   ADD following command in CMakeLists.txt of project,
#   given new structure of project (Catch_tests and project_jj_lib directories added ...)
add_executable(Catch_tests_run Catch_tests/AbsoluteTests.cpp project_jj_lib/DataTypes.cpp project_jj_lib/DataTypes.h project_jj_lib/Utils.cpp project_jj_lib/Utils.h project_jj_lib/LinkedList.cpp project_jj_lib/LinkedList.h project_jj_lib/Stack.cpp project_jj_lib/Stack.h project_jj_lib_part2/Datatypes_part2.cpp project_jj_lib_part2/Datatypes_part2.h)

#   in order to RUN tests
#   CREATE CONFIGURATION following path: Run > Edit Configurations > Templates > Catch
#   ADD tags of each TEST_CASE ([tag1][tag2]...) in Tags
#   and select Catch_tests_run in Target
#   [build, run/debug accordingly]
#   >>>>>>>>>>>>>>>>