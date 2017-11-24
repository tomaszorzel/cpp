#include <iostream>

using namespace std;

int t;
long long pesel;
int a_pesel[11];
int sum;

int main()
{
    cin >> t;
    while(t)
    {
        cin >> pesel;

        for(int i=10; i>=0; i--)
        {
            a_pesel[i] = pesel % 10;
            pesel = pesel/10;
        }

        if (pesel != 0) return 0;

        sum = a_pesel[0]*1 + a_pesel[1]*3 + a_pesel[2]*7 + a_pesel[3]*9 + a_pesel[4]*1 + a_pesel[5]*3 + a_pesel[6]*7 + a_pesel[7]*9 + a_pesel[8]*1 + a_pesel[9]*3 + a_pesel[10]*1;

        if(sum % 10)
        {
            cout << "N" << endl;
        }
        else
        {
            cout << "D" << endl;
        }

        t--;
    }

    return 0;
}
