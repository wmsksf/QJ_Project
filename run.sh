#!/bin/bash

mkdir -p build
mkdir -p build/obj

make

mv *.o build/obj
mv SortMergeJoin Catch_tests_run build/

#./input.sh <filename.init> <filename.work> | ./<executable> <<(append) output.sh
