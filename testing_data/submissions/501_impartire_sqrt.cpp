#include <bits/stdc++.h>
using namespace std;

int main() {
    int x;  cin >> x;
    
    if (x < 1)
    {
        volatile int a = 0;
        volatile int b = x / a;
        cout << b << '\n';
    }
    else if (x == 2)
    {
        int a[2] = {50, 51};
        cout << a[2] << '\n';
    }
    else if (x == 3)
    {
        int a[2] = {50, 51};
        cout << a[-1] << '\n';
    }
    else if (x > 8)
    {
        cout << sqrt(x - 10) << '\n';
    }
    else if (x == 8)
    {
        volatile int* p = nullptr;
        *p = 5;
    }
    else if (x == 7)
    {
        stack<int> st;
        cout << st.top() << '\n';
    }
    else if (x == 6)
    {
        int nex[10] = {0,1,2,3,4,5,6,7,8,9};
        function<void(int,int)> dfs = [&](int nod, int dad) {
            if (nex[nod] != dad) dfs(nex[nod], nod);
        };  dfs(0, -1);
    }
    else if (x == 5)
    {
        volatile int* p = new int;
        delete p;
        delete p;
    }
    else if (x == 4)
    {
        close(99999);
    }
}
