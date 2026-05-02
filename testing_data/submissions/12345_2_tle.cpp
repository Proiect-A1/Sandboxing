#include <iostream>
#include <set>

std::set < int > t;

int main()
{
    int x; std::cin >> x;
    long long s = 0;

    int it = 0;

    for(int i = 1 ; i <= 100000000 ; i++)
    {
        if(i % 3 == 2)
        {
            it++;
        }
        else if(i % 10 == 10)
        {
            it--;
        }
        else if(i % 7 == 5)
        {
            it++;
        }

        t.insert(i);
        
        if(i % 11 == 3)
            it++;

        s += it;
    }

    std::cout << x << std::endl;
}