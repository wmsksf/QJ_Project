#!bin/bash

echo "make multiple (Main project, Testing) executables..."
echo ""
echo "make SortMergeJoin"
TARGET_EXEC='SortMergeJoin' BUILD_DIR='./build' SRC_DIRS='./project_jj_lib' make
echo ""
echo "make Catch_tests_run"
# dpnd from *.o !main.o of project_jj cp  not working (subshells etc even with . ./mult_Make.sh)
TARGET_EXEC=
echo ""
echo "make DONE"