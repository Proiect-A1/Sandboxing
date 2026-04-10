#!/bin/bash

if [ $(whoami) != "root" ]
then 
    echo "Needs root privileges"
    exit 2
fi

if [ $# -ne 2 ]
then 
    echo "Provide number of amarati and the .rc file"
    exit 1
fi

if [ ! -f $2 ]
then 
    echo "Invalid .rc file"
    exit 2
fi

declare -A folders=(["problem"]="./problem_data" ["input"]="./inputs" ["correct"]="./correct_outputs" ["test"]="./test_scripts"  ["runs"]="./runs"  ["submissions"]="./submissions")

echo Creating folders...

setfacl -m d:o::r-x .
setfacl -m d:g:sudo:rwx .

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

num=$1


for i in $(seq 1 $num) 
do 
    name_amarat=amarat$i
    useradd $name_amarat 2> /dev/null
    uid_amarat=$(cat /etc/passwd | grep -w $name_amarat | cut -d : -f 3)

    mkdir ./runs/$name_amarat 2> /dev/null 
    chmod 0775 ./runs/$name_amarat 

    setfacl -m d:u:$uid_amarat:r-x ./runs
    setfacl -m d:u:$uid_amarat:r-x ./inputs
    setfacl -m d:u:$uid_amarat:rwx ./runs/$name_amarat 
    setfacl -m d:u:$uid_amarat:--- ./correct_outputs
    setfacl -m d:u:$uid_amarat:--- ./test_scripts
    setfacl -m d:u:$uid_amarat:--- ./problem_data
    setfacl -m d:u:$uid_amarat:--- ./submissions

    setfacl -m u:$uid_amarat:r-x ./runs
    setfacl -m u:$uid_amarat:r-x ./inputs
    setfacl -m u:$uid_amarat:rwx ./runs/$name_amarat 
    setfacl -m u:$uid_amarat:--- ./correct_outputs
    setfacl -m u:$uid_amarat:--- ./test_scripts
    setfacl -m u:$uid_amarat:--- ./problem_data
    setfacl -m u:$uid_amarat:--- ./submissions

    iptables -I OUTPUT -m owner --uid-owner $uid_amarat -j DROP
    usermod --password $(echo $name_amarat | openssl passwd -1 -stdin) $name_amarat

    name_marat=marat$i
    useradd $name_marat 2> /dev/null
    uid_marat=$(cat /etc/passwd | grep -w $name_marat | cut -d : -f 3)

    setfacl -m d:u:$uid_marat:r-x ./runs
    setfacl -m d:u:$uid_marat:r-x ./inputs
    setfacl -m d:u:$uid_marat:r-x ./runs/$name 
    setfacl -m d:u:$uid_marat:r-x ./correct_outputs
    setfacl -m d:u:$uid_marat:--- ./test_scripts
    setfacl -m d:u:$uid_marat:--- ./problem_data
    setfacl -m d:u:$uid_marat:--- ./submissions

    setfacl -m u:$uid_marat:r-x ./runs
    setfacl -m u:$uid_marat:r-x ./inputs
    setfacl -m u:$uid_marat:r-x ./runs/$name 
    setfacl -m u:$uid_marat:r-x ./correct_outputs
    setfacl -m u:$uid_marat:--- ./test_scripts
    setfacl -m u:$uid_marat:--- ./problem_data
    setfacl -m u:$uid_marat:--- ./submissions

    iptables -I OUTPUT -m owner --uid-owner $uid_marat -j DROP
    usermod --password $(echo $name_marat | openssl passwd -1 -stdin) $name_marat

    echo "$name_amarat & $name_marat"
done

if [ $(grep -c SANDBOX_WORKERS $2) -ne 0 ]
then 
    sed -i -r "s/SANDBOX_WORKERS=[0-9]*/SANDBOX_WORKERS=$num/" $2 
else 
    echo "export SANDBOX_WORKERS=$num" >> $2
fi


if [ $(grep -c SANDBOX_PATH $2) -ne 0 ]
then
    path=$(pwd | sed -E 's/\//\\\//g')
    sed -i -r "s/SANDBOX_PATH=(\/.*)+/SANDBOX_PATH=$path/" $2 
else 
    echo "export SANDBOX_PATH=$(pwd)" >> $2
fi

if [ $(echo $2 | grep -c /.bashrc) -eq 1 ]
then 
    source $2
fi 

cat $2 | grep SANDBOX
echo Done

