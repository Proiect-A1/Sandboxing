function startWatch(){
    tbegin=$(date +"%s%3N")
}
function stopWatch(){
    tend=$(date +"%s%3N")

    echo "[$1] Time elapsed: $((tend-tbegin))ms"
}

function run(){
    echo "Running for version ($1)..."

    cp ${1}/problem.h problem.h
    startWatch
    g++ -std=c++17 -O2 -o gen gen.cpp
    g++ -std=c++17 -O2 -o val val.cpp
    g++ -std=c++17 -O2 -o checker checker.cpp
    g++ -std=c++17 -O2 -o main main.cpp
    stopWatch "compilation"

    NMAX=1000000

    startWatch
    ./gen $NMAX > in.txt
    stopWatch "generation"

    if [ $? -ne 0 ]; then
        echo "generation failed"
        exit $?
    fi

    startWatch
    ./val $NMAX < in.txt
    stopWatch "validation"
    if [ $? -ne 0 ]; then
        echo "validation failed"
        exit $?
    fi

    startWatch
    ./main < in.txt > ok.txt
    stopWatch "main"

    if [ $? -ne 0 ]; then
        echo "main solution failed"
        exit $?
    fi

    startWatch
    ./checker in.txt ok.txt ok.txt main.cpp &> /dev/null
    stopWatch "checker"

    startWatch
    rm checker
    rm gen
    rm val
    rm main
    rm problem.h
    stopWatch "cleanup"
}

run problemh-1.0
run problemh-2.0

    
