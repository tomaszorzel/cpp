#include <iostream>

using namespace std;

int main()
{
    int D=0;
    cin >> D;
    while (D)
    {
        int a,b;
        cin >> a >> b;
        int result = 1;
        if (a%10 == 2)
        {
            if (b%4 == 1) result = 2;
            if (b%4 == 2) result = 4;
            if (b%4 == 3) result = 8;
            if (b%4 == 0) result = 6;
        }
        else if (a%10 == 3)
        {
            if (b%3 == 1) result = 3;
            else if (b%3 == 2) result = 9;
            else if (b%3 == 0) result = 7;
        }
        else if (a%10 == 4)
        {
            if (b%2 == 0) result = 6;
            else result = 4;
        }
        else if (a%10 == 5)
        {
            result = 5;
        }
        else if (a%10 == 6)
        {
            result = 6;
        }
        else if (a%10 == 7)
        {
            if (b%4 == 1) result = 7;
            else if (b%4 == 2) result = 9;
            else if (b%4 == 3) result = 3;
            else if (b%4 == 0) result = 1;
        }
        else if (a%10 == 8)
        {
            if (b%4 == 1) result = 8;
            else if (b%4 == 2) result = 4;
            else if (b%4 == 3) result = 2;
            else if (b%4 == 0) result = 6;
        }
        else if (a%10 == 9)
        {
            if (b%2 == 1) result = 9;
            else if (b%2 == 0) result = 1;
        }
        else if (a%10 == 0)
        {
            result = 0;
        }
        cout << result << endl;
        D--;
    }
    return 0;
}
