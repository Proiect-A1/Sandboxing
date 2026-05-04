#include "problem.h"
#include<bits/stdc++.h>

using namespace std;
const int NMAX=105;
int maxign = 10'000;
int n;
int p[NMAX], tmp[NMAX];
int readAndCheckAns(int tid, validator* in){
    int ops = in->readInt(0, maxign, 1);

    for(int i=1;i<=n;i++) tmp[i] = p[i];

    for(int i=0;i<ops;i++){
        int l = in->readInt(1, n, 1);
        int r = in->readInt(1, n, 1);
        in->failIf(l == r, "l and r must be different (test case %d)", tid);
        swap(tmp[l], tmp[r]);
    }
    for(int i=1;i<=n;i++){
        in->failIf(tmp[i]!=i, "permutation has not been sorted (test case %d)", tid);
    }
    return ops;
}
void check_for_pragmas(checker& c, char* solution_path){
    FILE* fin = fopen(solution_path, "r");
    char buff[MAX_SUBMISSION_SIZE+1]{};
    fread(buff, 1, MAX_SUBMISSION_SIZE, fin);
    if(strstr(buff, "#pragma"))
        c.quitf(FAIL, "#pragma found in source code");
    if(strstr(buff, "__attribute__"))
        c.quitf(FAIL, "__attribute__ found in source code");
}
int main(int argc, char** argv){
    checker c(argc, argv);
    check_for_pragmas(c, argv[CHECKER_SOLUTION_ARG]);
    int maxt = atoi(argv[CHECKER_FIRST_ARG]);
    int maxn = atoi(argv[CHECKER_FIRST_ARG+1]);
    maxign = atoi(argv[CHECKER_FIRST_ARG+2]);
    int t=c.in.readInt(1, maxt, 1);
    float points = 1.0;
    bool found_better_ans = false;
    for(int i=0;i<t;i++){
        n = c.in.readInt(1, maxn, 1);
        for(int j=1; j<=n; j++){
            p[j] = c.in.readInt(1, n, 1);
        }
        int participant = readAndCheckAns(i+1, (validator*)&c.out);
        int correct = readAndCheckAns(i+1, (validator*)&c.ok);
        if(participant < correct){
            points = min(points, 1.0f);
            found_better_ans = true;
        }
        else if(participant == correct){
            points = min(points, 1.0f);
        }
        else if(participant <= 100){
            points = min(points, 0.8f);
        }
        else if(participant <= 2'000){
            points = min(points, 0.5f);
        }
        else if(participant <= maxign){
            points = min(points, 0.1f);
        }
        else points = 0.0;
    }
    if(points == 1.0 && found_better_ans)
        c.pquitf(SUPER, points, "participant has found better answer than the jury");
    else if(points == 1.0)
        c.pquitf(OK, points, "ok");
    else if(points == 0.0)
        c.pquitf(WA, points, "too many swaps used");
    else
        c.pquitf(PA, points, "ok, factor=%.2f", points);
    return 0;
}