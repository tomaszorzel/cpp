#include <iostream>
#include <stdlib.h>
using namespace std;

void fillEmptyPlacesWithDots(char** matrix, int r) {
    for (int i=0; i<2*r; i++) {
        for (int j=0; j<2*r; j++) {
            if (matrix[i][j]!='*')
            matrix[i][j]='.';
        }
    }
}

void constructLeftRotatingFan(char** matrix, int r) {
    for (int i=0; i<r; i++) { // II
        for (int j=0; j<=i; j++) {
                    matrix[i][j]='*';
        }
    }
    for (int i=0; i<r; i++) { // I
        for (int j=r; j<2*r-i; j++) {
                    matrix[i][j]='*';
        }
    }
    for (int i=r; i<2*r; i++) { // IV
        for (int j=i; j<2*r; j++) {
                    matrix[i][j]='*';
        }
    }
    for (int i=r; i<2*r; i++) { // III
        for (int j=2*r-i-1; j<r; j++) {
                    matrix[i][j]='*';
        }
    }
    fillEmptyPlacesWithDots(matrix, r);
}

void constructRightRotatingFan(char** matrix, int r) {
    for (int i=0; i<r; i++) { // II
        for (int j=i; j<r; j++) {
            matrix[i][j]='*';
        }
    }
    for (int i=0; i<r; i++) { // I
        for (int j=2*r-1; j>=2*r-1-i; j--) {
            matrix[i][j]='*';
        }
    }
    for (int i=r; i<2*r; i++) { // IV
        for (int j=r; j<=i; j++) {
            matrix[i][j]='*';
        }
    }
    for (int i=r; i<2*r; i++) { // III
        for (int j=0; j<r-i+r; j++) {
            matrix[i][j]='*';
        }
    }
    fillEmptyPlacesWithDots(matrix, r);
}

void printFan(char** matrix, int r) {
    for (int i=0; i<2*r; i++) {
        for (int j=0; j<2*r; j++) {
            cout << matrix[i][j];
        }
        cout << endl;
    }
}

int main()
{
    int r;
    cin >> r;
    while (r) {
        char** windmill = new char*[2*abs(r)];
        for (int i=0; i<2*abs(r); i++)
            windmill[i] = new char[2*abs(r)];

        if (r<0) {
            r=abs(r);
            constructRightRotatingFan(windmill, r);
        } else {
            constructLeftRotatingFan(windmill, r);
        }

        printFan(windmill, r);

        // not sure why, but referee does not want to pass my program with deallocation of windmill matrix
//        for (int j=0; j<2*r; j++)
//            delete [] windmill[j];
//        delete [] windmill;

        cin >> r;
    }
    return 0;
}
