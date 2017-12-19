#include <iostream>

using namespace std;

int main()
{
    int providedNumber;
    cin >> providedNumber;
    if (providedNumber==0)
        cout << "0" << endl;
    else if (providedNumber<=2)
        cout << "NIE" << endl;
    else {
        int temp=providedNumber; // will be used to print all odd numbers
        cout << "1 "; // in output firstly print "1 "
        if (temp%2==0)
            temp-=1;
        while (temp>=3) {
            cout << temp << " "; // then print all odd numbers till reach 3
            temp-=2;
        }

        cout << "0 ";
        temp=2; // will be used to print all even numbers
        int temp2;
        if (providedNumber%2>0) // if providedNumber is even then it will be the last printed number. If not, then providedNumber-1 will be last in output
            temp2=providedNumber-1;
        else
            temp2=providedNumber;
        while (temp<=temp2) { // till reach max even number (providedNumber or providedNumber-1)
            if (temp!=temp2)
                cout << temp << " ";
            else
                cout << temp << endl;
            temp+=2;
        }
    }
}
