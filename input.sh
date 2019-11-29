#!/bin/bash

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

#./input.sh <filename.init> <filename.work>