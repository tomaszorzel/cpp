#include <iostream>
using namespace std;

bool isPrimary(int number);

int main()
{
    int noOfTests;

    cin >> noOfTests;
    int numbers[noOfTests];

    for (int i = 0; i < noOfTests; i++)
    {
        cin >> numbers[i];
    }

    for (int i = 0; i < noOfTests; i++)
    {
        if (isPrimary(numbers[i]))
            cout << "TAK" << endl;
        else
            cout << "NIE" << endl;
    }
    return 0;
}

bool isPrimary(int number)
{
    bool isPrimary = true;
    if (number > 1)
    {
        for (int i = 2; i*i <= number; i++)
        {
            if (number%i == 0)
            {
                isPrimary = false;
                break;
            }
        }
    }
    else
    {
        isPrimary = false;
    }

    return isPrimary;
}
