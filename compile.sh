#!/bin/bash

mkdir -p build
mkdir -p build/obj

make

mv *.o build/obj
mv ExecQueries Catch_tests_run build/
cp input.sh output.sh run.sh build/
