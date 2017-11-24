#include <iostream>

using namespace std;

int main()
{
    int D=0;
    cin >> D;
    while (D)
    {
        int n;
        cin >> n;
        int i = n;
        int silnia = 1;
        int d = 0, j=0;

        if (n < 10)
        {
            while (i > 1)
            {
                silnia *= i--;
            }
            j = silnia % 10;
            d = (silnia % 100) / 10;
        }

        cout << d << " " << j << endl;
        D--;
    }
    return 0;
}
