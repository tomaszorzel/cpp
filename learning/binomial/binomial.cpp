#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
    int t;
    cin >> t;

    while (t)
    {
        int n,k;
        cin >> n >> k;

        if ( 2*k > n ) k=n-k;

        long long result=1;
        for (int i=1; i<=k; i++)
        {
            result = result*(n-i+1)/i;
        }

        //cout << setprecision(1000000);
        cout << result << endl;

        t--;
    }
    return 0;
}
