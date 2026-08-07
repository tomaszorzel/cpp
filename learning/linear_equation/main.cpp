#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
    float a, b, c;
    cin >> a >> b >> c;
    if (a==0 && (c-b)==0)
        cout << "NWR" << endl;
    else if (a==0 && (c-b)!=0)
        cout << "BR" << endl;
    else {
        cout << fixed << setprecision(2) << (c-b)/a << endl;
    }
    return 0;
}
