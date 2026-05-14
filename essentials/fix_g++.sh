#!/bin/bash

nr=0

for i in $(g++ -print-search-dirs)
do
    nr=$((nr + 1))

    if [ $((nr % 2)) -eq 1 ]
    then 
        continue 
    fi

    str=$i

    if [ ${i:0:1} == '=' ]
    then 
        str=${i:1:-1}
    fi

    last_idx=0

    for((j = 0 ; j <= ${#str} ; j++));
    do 

        if [ $j -eq ${#str} ] || [ ${str:j:1} == ':' ]
        then
            path=${str:last_idx:$((j - last_idx))}
            echo $path
            cp -r --parents $path .
            last_idx=$((j + 1))
        fi
    done
done