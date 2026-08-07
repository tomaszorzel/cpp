#include <iostream>
using namespace std;

char* string_merge(char *, char *);

int main()
{
    int numberOfTests;
    cin >> numberOfTests;
    int velocities[numberOfTests*2];
    int avarageVelocities[numberOfTests];

    for (int i=0; i<numberOfTests*2; i++)
    {
        cin >>  velocities[i];
    }

    for (int i=0, j=0; i<numberOfTests, j<numberOfTests*2; i++, j+=2)
    {
        avarageVelocities[i] = 2*(velocities[j]*velocities[j+1])/(velocities[j]+velocities[j+1]);
        cout << avarageVelocities[i] << endl;
    }
}
