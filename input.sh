#!/bin/bash

init=$1
#path=$2
work=$2

#awk '$0="$path"$0' $init > $init

while read -r line; do
echo $line
done < $init
echo 'DONE'

while IFS= read -r line; do
echo "$line"
done < $work
echo 'FF'

#./input.sh <filename.init> <path_to_filename> <filename.work>