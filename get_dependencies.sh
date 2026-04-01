#!/bin/bash

if [ $# -ne 1 ]
then 
	echo 'error'
	exit 1
fi

app=$1

list=$(ldd $app | egrep -o '/lib.*\.[0-9]') 

for i in $list 
do
	file=$i
	sudo mkdir -p .$(dirname $file)
	sudo cp $file .$file
done
