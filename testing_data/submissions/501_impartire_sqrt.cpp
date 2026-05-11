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
        volatile int a[2] = {50, 51};
        cout << a[(int) round(3 + sin(x*x))] << '\n';
    }
    else if (x == 3)
    {
        volatile int a[2] = {50, 51};
        cout << a[(int) round(-2 + sin(x*x))] << '\n';
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
        volatile int nex[10] = {0,1,2,3,4,5,6,7,8,9};
        function<void(int,int)> dfs = [&](int nod, int dad) {
            dfs(nex[nod], nod);
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
