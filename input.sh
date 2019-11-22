#!/bin/bash

filename=$1
while read -r line; do
# read each line
echo $line
done < $filename

# ./input.sh | ./executable <<(append) output.sh? !but gets r0r1r2.... yet to absorb line by line

