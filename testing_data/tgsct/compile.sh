#!/bin/bash

problemhpath=${SANDBOX_PATH}/../problem_framework/problem.h

path=${SANDBOX_PATH}/problem_data/$1

echo $problemhpath
echo $path

# cd $path
for subfolder in "files/checkers" "files/validators" "files/generators" "files/sources"; do 
    fullpath=${path}/${subfolder}
    cp ${problemhpath} ${fullpath}/problem.h
    for fn in "$fullpath"/*.cpp; do
        base=${fn%.cpp}
        echo $base
        echo $fn
        # cp problemhpath
        g++ -O2 -std=c++17 -o $base $fn
    done
    rm ${fullpath}/problem.h
done
