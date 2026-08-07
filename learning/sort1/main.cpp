#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
using namespace std;

class Point {
public:
    string name;
    int x;
    int y;
    double distance;

    void operator=(const Point& b) {
        name = b.name;
        x = b.x;
        y = b.y;
        distance = b.distance;
    }
};

bool operator<(Point a, Point b) {
    if ((a.x*a.x + a.y*a.y) < (b.x*b.x + b.y*b.y))
        return true;
    else
        return false;
}

void sortPoints(Point* points, int length) {
    int changed=0;
    do {
        for (int i=0; i<(length-1); i++) {
            if (points[i] < points[i+1]) {
                Point tempPoint = points[i];
                points[i] = points[i+1];
                points[i+1] = tempPoint;
            }
        }
    } while (changed!=0);
}

int main() {
    int t;
    cin >> t;
    while (t--)
    {
        int howManyPoints;

        cin >> howManyPoints;
        Point* points = new Point[howManyPoints];
        vector<Point> vectorOfPoints(points,points+howManyPoints);

        for (int i=0; i<howManyPoints; i++) {
            cin >> vectorOfPoints[i].name >> vectorOfPoints[i].x >> vectorOfPoints[i].y;
            vectorOfPoints[i].distance = sqrt(points[i].x*points[i].x + points[i].y*points[i].y);
        }

        sort(vectorOfPoints.begin(), vectorOfPoints.end());

        for (int i=0; i<howManyPoints; i++) {
            cout << vectorOfPoints[i].name << " " << vectorOfPoints[i].x << " " << vectorOfPoints[i].y << endl;
        }

        delete [] points;
    }
    return 0;
}
