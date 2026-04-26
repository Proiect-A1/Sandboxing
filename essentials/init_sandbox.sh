#!/bin/bash

if [ $(whoami) != "root" ]
then 
    echo "Needs root privileges"
    exit 2
fi

if [ $# -ne 1 ]
then 
    echo "Provide number of amarati"
    exit 1
fi

declare -A folders=(["problem"]="./problem_data" ["input"]="./inputs" ["correct"]="./correct_outputs" ["test"]="./test_scripts"  ["runs"]="./runs"  ["submissions"]="./submissions"  ["tmp"]="./tmp")

echo Creating folders...

setfacl -m d:o::r-x .
setfacl -m d:g:sudo:rwx .
setfacl -m d:g:root:rwx .

for i in ${folders[@]}
do 
    if [ -d $i ]
    then 
        rm -r $i
        echo Deleted $i
    fi

    mkdir $i
    chmod 0775 $i
    echo Created $i
done

echo Done


echo Creating Users...

groupadd amarati 2> /dev/null
groupadd marati 2> /dev/null

setfacl -m g:amarati:--x .
setfacl -m g:marati:--x .

#./runs
setfacl -m g:amarati:--x ./runs
setfacl -m d:g:amarati:--- ./runs

setfacl -m g:marati:--x ./runs
setfacl -m d:g:marati:--- ./runs

#./inputs
setfacl -m g:amarati:r-x ./inputs
setfacl -m g:marati:r-x ./inputs

setfacl -m d:g:amarati:r-x ./inputs
setfacl -m d:g:marati:r-x ./inputs

#./correct_outputs
setfacl -m g:amarati:--- ./correct_outputs
setfacl -m g:marati:r-x ./correct_outputs
setfacl -m d:g:marati:r-x ./correct_outputs

#./test_scripts
setfacl -m g:amarati:r-x ./test_scripts
setfacl -m d:g:amarati:r-x ./test_scripts

setfacl -m g:marati:--- ./test_scripts

#./problem_data
setfacl -m g:amarati:--- ./problem_data
setfacl -m g:marati:--- ./problem_data

#./submissions
setfacl -m g:amarati:--- ./submissions
setfacl -m g:marati:--- ./submissions

#./tmp
setfacl -m g:amarati:--- ./tmp
setfacl -m g:marati:--- ./tmp

num=$1


for i in $(seq 1 $num) 
do 
    #amarat
    name_amarat=amarat$i
    useradd $name_amarat 2> /dev/null
    usermod -a -G amarati $name_amarat 2> /dev/null
    uid_amarat=$(cat /etc/passwd | grep -w $name_amarat | cut -d : -f 3)
    iptables -I OUTPUT -m owner --uid-owner $uid_amarat -j DROP
    usermod --password $(echo $name_amarat | openssl passwd -1 -stdin) $name_amarat
    usermod -s /bin/bash $name_amarat 2>&1 > /dev/null

    mkdir ./runs/$name_amarat 2> /dev/null 

    setfacl -m u:$name_amarat:rwx ./runs/$name_amarat
    setfacl -m d:u:$name_amarat:rwx ./runs/$name_amarat

    #marat
    name_marat=marat$i
    useradd $name_marat 2> /dev/null
    usermod -a -G marati $name_marat 2> /dev/null
    uid_marat=$(cat /etc/passwd | grep -w $name_marat | cut -d : -f 3)
    iptables -I OUTPUT -m owner --uid-owner $uid_marat -j DROP
    usermod --password $(echo $name_marat | openssl passwd -1 -stdin) $name_marat
    usermod -s /bin/bash $name_marat 2>&1 > /dev/null

    setfacl -m u:$name_marat:rwx ./runs/$name_amarat
    setfacl -m d:u:$name_marat:rwx ./runs/$name_amarat

    echo "$name_amarat & $name_marat"
done

env_path="/etc/environment"

if [ $(grep -c SANDBOX_WORKERS $env_path) -ne 0 ]
then 
    sed -i -r "s/SANDBOX_WORKERS=[0-9]*/SANDBOX_WORKERS=$num/" $env_path 
else 
    echo "SANDBOX_WORKERS=$num" >> $env_path
fi


if [ $(grep -c SANDBOX_PATH $env_path) -ne 0 ]
then
    path=$(pwd | sed -E 's/\//\\\//g')
    sed -i -r "s/SANDBOX_PATH=(\/.*)+/SANDBOX_PATH=$path/" $env_path 
else 
    echo "SANDBOX_PATH=$(pwd)" >> $env_path
fi

source $env_path 

cat $env_path | grep SANDBOX
echo Done

