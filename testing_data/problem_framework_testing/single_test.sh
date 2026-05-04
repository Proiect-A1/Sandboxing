g++ -std=c++14 -o gen gen.cpp
g++ -std=c++14 -o val val.cpp
g++ -std=c++14 -o checker checker.cpp
mainpath="$2"
partpath="$1"
mainexec="${mainpath%.*}"
partexec="${partpath%.*}"
echo "$mainexec"
echo "$partexec"
g++ -std=c++14 -o $mainexec $mainpath
g++ -std=c++14 -o $partexec $partpath

./gen 100 100 > in.txt

if [ $? -ne 0 ]; then
    echo "generation failed"
    exit $?
fi

./val 100 100 < in.txt
if [ $? -ne 0 ]; then
    echo "validation failed"
    exit $?
fi

./$mainexec < in.txt > ok.txt

if [ $? -ne 0 ]; then
    echo "main solution failed"
    exit $?
fi

./$partexec < in.txt > out.txt

if [ $? -ne 0 ]; then
    echo "participant solution failed"
    exit $?
fi

./checker in.txt out.txt ok.txt $partpath 100 100 10000

    
