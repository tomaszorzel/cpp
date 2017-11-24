#include <iostream>

using namespace std;

int main()
{
    int t, n, x, y;
    cin >> t; /* read number of tests */
    while (t)
    {
        cin >> n >> x >> y;
        for (int i = 2; i < n; i++)
        {
            if ( ((i%x) == 0) && ((i%y) != 0) ) cout << i << " ";
        }
        cout << endl;
        t--;
    }
    return 0;
}
