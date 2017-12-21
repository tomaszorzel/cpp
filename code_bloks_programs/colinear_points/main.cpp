#include <iostream>

using namespace std;

int main()
{
    int t;
    cin >> t;
    while (t--) {
        int x1, y1, x2, y2, x3, y3;
        cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;

        // Use area of triangle to check if given points are collinear
        int areaOfTriangle = (x1-x2)*(y2-y3) - (x2-x3)*(y1-y2);
        if (areaOfTriangle==0)
            cout << "TAK" << endl;
        else
            cout << "NIE" << endl;
    }
    return 0;
}
