#include <iostream>
#include <cstdlib>
using namespace std;

class Point {
public:
    int x;
    int y;

    Point() {
        x=0;
        y=0;
    }
};

int main()
{
    int t;
    cin >> t;
    while (t--) {
        int howManyHints, direction=0, steps=0;
        Point endingPoint;

        cin >> howManyHints;
        while (howManyHints--) {
            cin >> direction >> steps;

            switch (direction) {
            case 0:
                endingPoint.y+=steps;
                break;
            case 1:
                endingPoint.y-=steps;
                break;
            case 2:
                endingPoint.x-=steps;
                break;
            case 3:
                endingPoint.x+=steps;
            default:
                break;
            }
        }

        if (endingPoint.x==0 && endingPoint.y>0) {
            direction=0;
            cout << direction << " " << endingPoint.y << endl;
        } else if (endingPoint.x==0 && endingPoint.y<0) {
            direction=1;
            cout << direction << " " << abs(endingPoint.y) << endl;
        } else if (endingPoint.y==0 && endingPoint.x>0) {
            direction=3;
            cout << direction << " " << endingPoint.x << endl;
        } else if (endingPoint.y==0 && endingPoint.x<0) {
            direction=2;
            cout << direction << " " << abs(endingPoint.x) << endl;
        } else if (endingPoint.y==0 && endingPoint.x==0) {
            cout << "studnia" << endl;
        } else if (endingPoint.x>0 && endingPoint.y>0) {
            direction=0;
            cout << direction << " " << endingPoint.y << endl;
            direction=3;
            cout << direction << " " << endingPoint.x << endl;
        } else if (endingPoint.x<0 && endingPoint.y>0) {
            direction=0;
            cout << direction << " " << endingPoint.y << endl;
            direction=2;
            cout << direction << " " << abs(endingPoint.x) << endl;
        } else if (endingPoint.x<0 && endingPoint.y<0) {
            direction=1;
            cout << direction << " " << abs(endingPoint.y) << endl;
            direction=2;
            cout << direction << " " << abs(endingPoint.x) << endl;
        } else if (endingPoint.x>0 && endingPoint.y<0) {
            direction=1;
            cout << direction << " " << abs(endingPoint.y) << endl;
            direction=3;
            cout << direction << " " << endingPoint.x << endl;
        }
    }
    return 0;
}
