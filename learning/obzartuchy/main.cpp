#include <iostream>
using namespace std;

int main()
{
    int numberOfTests;
    cin >> numberOfTests;

    for (int i=0; i<numberOfTests; i++)
    {
        int howManyGuzzlers, cookiesInAPackage, eatenCookies=0, secondsInADay=86400, packages;
        cin >> howManyGuzzlers;
        int timeToEatOneCookie[howManyGuzzlers];

        cin >> cookiesInAPackage;

        for (int j=0; j<howManyGuzzlers; j++)
        {
            cin >> timeToEatOneCookie[j];
        }

        for (int j=0; j<howManyGuzzlers; j++)
        {
            eatenCookies += secondsInADay / timeToEatOneCookie[j];
        }

        packages = eatenCookies / cookiesInAPackage;
        if (eatenCookies % cookiesInAPackage > 0)
        {
            packages += 1;
        }

        cout << packages << endl;
    }
    return 0;
}
