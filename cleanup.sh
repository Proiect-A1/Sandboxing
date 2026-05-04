#!/bin/bash 

if [ $(whoami) != "root" ]
then 
    echo "Needs root privileges"
    exit 2
fi


echo "Bye bye marati :("
groupdel marati
echo "Bye bye amarati :("
groupdel amarati

for i in $(seq 1 1 $SANDBOX_WORKERS)
do 
    name_amarat="amarat$i"
    name_marat="marat$i"
    
    userdel $name_amarat
    userdel $name_marat
done

rm -r $SANDBOX_PATH 

sed -i -r "s/SANDBOX_PATH=(\/.*)+//" "/etc/environment" 
sed -i -r "s/SANDBOX_WORKERS=[0-9]*//" "/etc/environment"

source /etc/environment

echo "Cleaned up :(((("


