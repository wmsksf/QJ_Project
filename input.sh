#!/bin/bash

#dir=$1
#init=$(find $dir -name *.init)
#work=$(find $dir -name *.work)

init=$1
work=$2

while read -r line; do
# read each line
echo $line
done < $init
echo 'DONE'

while IFS= read -r line; do
# read each line
echo "$line"
done < $work
echo 'FF'

#./input.sh <filename.init> <filename.work>