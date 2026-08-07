#include <iostream>

using namespace std;

inline double SQR(double x)
{
    return x*x;
}

int main()
{
    double r, d, sqr_c, S, pi=3.141592654;
    cin >> r >> d;
    sqr_c = SQR(r) - SQR(d/2);
    S = pi*sqr_c;
    cout.setf(ios::fixed);
    cout.precision(2);
    cout << S << endl;
    return 0;
}
