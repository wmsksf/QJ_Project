#!/bin/bash

init=$1
work=$2

while read -r line; do
echo $line
done < $init
echo 'DONE'

while IFS= read -r line; do
echo "$line"
done < $work
echo 'FF'

#./input.sh <filename.init> <filename.work>