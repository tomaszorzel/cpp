#include <iostream>
using namespace std;

int main()
{
    int t,a,b,c,result;

    cin >> t;

    if (t<1 || t>10) return 0;

    while (t)
    {
        cin >> a >> b;

        if (a > 1000000000 || b > 1000000000) return 0;

        if (b == 0) result=1;
        if (a%10 == 0) result=0;
        if (a%10 == 1) result=1;

        if (a%10 == 2)
        {
            int c=b%4;
            if (c==1) result=2;
            else if (c==2) result=4;
            else if (c==3) result=8;
            else result=6;
        }

        if (a%10 == 3)
        {
            int c=b%4;
            if (c==1) result=3;
            else if (c==2) result=9;
            else if (c==3) result=7;
            else result=1;
        }

        if (a%10 == 4)
        {
            int c=b%2;
            if (c==1) result=4;
            else result=6;
        }

        if (a%10 == 5) result=5;

        if (a%10 == 6) result=6;

        if (a%10 == 7)
        {
            int c=b%4;
            if (c==1) result=7;
            else if (c==2) result=9;
            else if (c==3) result=3;
            else result=6;
        }

        if (a%10 == 8)
        {
            int c=b%4;
            if (c==1) result=8;
            else if (c==2) result=4;
            else if (c==3) result=2;
            else result=6;
        }

        if (a%10 == 9)
        {
            int c=b%2;
            if (c==1) result=9;
            else result=1;
        }

        cout << result << endl;

        t--;
    }

    return 0;
}
