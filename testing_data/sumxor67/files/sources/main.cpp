#include <bits/stdc++.h>

using namespace std;

int c,n,a[1000005],p[1000005],b[1000005];
int lft=0;
int f[1000005];
void makeb(int ans)
{
    for(int i=1;i<=n;i++)
        b[i]=p[a[i]];
    int suma=0;
    for(int i=1;i<=n;i++)
    {
        assert(f[b[i]]==0);
        assert(b[i]<=n&&b[i]>=1);
        f[b[i]]=1;
        suma^=(a[i]+b[i]);
    }
    assert(suma==ans);
    cout<<ans<<'\n';
    for(int i=1;i<=n;i++)
        cout<<b[i]<<' ';
    exit(0);
}
int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cin>>c>>n;
    for(int i=1;i<=n;i++)
        cin>>a[i];
    if(n==1)
    {
        cout<<2<<'\n'<<1;
        return 0;
    }
    if(c==1)
    {
        if(n%2==0)
        {
            for(int i=1;i<=n;i+=2)
            {
                p[i]=i+1;
                p[i+1]=i;
            }
            makeb(0);
        }
        else
        {
            p[1]=1;
            p[2]=2;
            p[3]=3;
            for(int i=4;i<=n;i+=2)
            {
                p[i]=i+1;
                p[i+1]=i;
            }
            makeb(0);
        }
        return 0;
    }
    lft=n;
    int ans=0;
    for(int i=0;(1<<i)<=n;i++)
    {
        ans+=(1<<i);
        p[(1<<i)]=(1<<i);
        lft--;
    }
    ans*=2;
    if(lft%2==1)
    {
        p[3]=3;
        p[1]=0;
        p[2]=0;
    }
    int last=0;
    for(int i=1;i<=n;i++)
        if(p[i]==0)
        {
            if(last==0)
                last=i;
            else
            {
                p[last]=i;
                p[i]=last;
                last=0;
            }
        }
    makeb(ans);
    return 0;
}
