#include <iostream>
using namespace std;

class Point {
    int x, y;
public:
    Point () {
        x=0;
        y=0;
    }
    Point (int a, int b) {
        x=a;
        y=b;
    }
    int getX() {
        return x;
    }
    int getY() {
        return y;
    }
};

int main()
{
    int t;
    cin >> t;
    while (t--) {
        Point points[3];
        int x, y, a=0, b=0;
        for (int i=0; i<3; i++) {
            cin >> x >> y;
            points[i] = Point(x,y);
        }
        a=
        b=points[1].getY()-a*points[1].getX();

        cout << "a=" << a << " b=" << b << endl;

    }
    return 0;
}
