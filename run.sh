#!/bin/bash

mkdir -p build
mkdir -p build/obj

make

mv *.o build/obj
mv ExecQueries build/
cp input.sh build/

#./input.sh <filename.init> <filename.work> | ./<executable> <<(append) output.sh
