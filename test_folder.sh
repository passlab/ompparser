#!/bin/bash
for filename in $1/*; do
    #echo "sss"
    ./tester "$filename" 
done
