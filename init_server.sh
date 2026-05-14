#!/bin/bash 

if [ $(whoami) != "root" ]
then 
    echo "Needs root privileges"
    exit 2
fi

bash ./essentials/init.sh 20

if [ $# -eq 1 -a $1 == 'debug' ] 
then 
    cd $SANDBOX_PATH
    cp ../essentials/get_dependencies.sh .
    mkdir -p $SANDBOX_PATH/bin ;  cp /bin/bash $SANDBOX_PATH/bin ;  bash $SANDBOX_PATH/get_dependencies.sh $SANDBOX_PATH/bin/bash
    mkdir -p $SANDBOX_PATH/usr/bin ;  cp /usr/bin/ls $SANDBOX_PATH/usr/bin/ ;  bash $SANDBOX_PATH/get_dependencies.sh $SANDBOX_PATH/usr/bin/ls
    mkdir -p $SANDBOX_PATH/usr/bin ;  cp /usr/bin/getfacl $SANDBOX_PATH/usr/bin/ ;  bash $SANDBOX_PATH/get_dependencies.sh $SANDBOX_PATH/usr/bin/getfacl
    #/usr/bin/getfacl
    rm get_dependencies.sh
fi
