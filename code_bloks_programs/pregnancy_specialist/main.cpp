#include <iostream>
#include <math.h>
using namespace std;

int main()
{
    int t;
    cin >> t;
    while (t--) {
        double x, y, z;
        cin >> x >> y >> z;
        double lengthOfPregnancy = (x*12+y*12-z*y*12)/(1-z);
        cout << round(lengthOfPregnancy) << endl;
    }
    return 0;
}
