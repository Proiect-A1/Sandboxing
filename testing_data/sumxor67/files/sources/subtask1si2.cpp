#include <bits/stdc++.h>

using namespace std;

int n,a[1000005],v[1000005],c,sol[1000005];
int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cin>>c;
    cin>>n;
    for(int i=1;i<=n;i++)
        cin>>a[i];
    for(int i=1;i<=n;i++)
        v[i]=i;
    int maxim=-1;
    int minim=1e9;
    do
    {
        int val=0;
        for(int i=1;i<=n;i++)
            val^=(a[i]+v[i]);
        if(c==1)
        {
            if(val<minim)
            {
                minim=val;
                for(int i=1;i<=n;i++)
                    sol[i]=v[i];
            }
        }
        else
        {
            if(val>maxim)
            {
                maxim=val;
                for(int i=1;i<=n;i++)
                    sol[i]=v[i];
            }
        }
    }while(next_permutation(v+1,v+n+1));
    if(c==1)
        cout<<minim<<'\n';
    else
        cout<<maxim<<'\n';
    for(int i=1;i<=n;i++)
        cout<<sol[i]<<' ';
    return 0;
}
