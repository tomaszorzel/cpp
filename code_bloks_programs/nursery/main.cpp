#include <iostream>

using namespace std;

int main()
{
    int t, a, b;
    cin >> t;
    while (t--)
    {
        cin >> a >> b;
        int lower = (a>b) ? b : a;
        int bigger = (a>b) ? a : b;
        while (lower%bigger != 0)
        {
            lower+=((a>b) ? b:a);
        }
        cout << lower << endl;
    }
    return 0;
}
