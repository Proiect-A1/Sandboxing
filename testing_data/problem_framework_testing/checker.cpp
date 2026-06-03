#include "problem.h"

int main(int argc, char** argv){
    checker c(argc, argv);
    int64_t out=c.out.readInt();
    int64_t ok=c.ok.readInt();
    if(out==ok) c.quitf(OK, "Correct");
    else c.quitf(WA, "Wrong answer");
    return 0;
}