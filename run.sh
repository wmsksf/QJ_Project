#!/bin/bash

mkdir -p build
mkdir -p build/obj

make

mv *.o build/obj
mv ExecQueries Catch_tests_run build/
cp input.sh build/

#./input.sh <filename.init> <filename.work> | ./<executable> <<(append) output.sh
