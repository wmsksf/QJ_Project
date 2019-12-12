#!/bin/bash

results=$1
statistics=$2

if [[ -e $results ]]; then
echo -n > $results
fi

while IFS= read -r line; do
echo "$line"
if [[ "$line" =~ "RESULT" ]]; then
r=$(echo $line | cut -d ':' -f 2)
echo "$r" >> $results
fi
done > $statistics

mkdir -p outfiles
mv $statistics $results outfiles

#./output.sh <filename.results> <filename.statistics>