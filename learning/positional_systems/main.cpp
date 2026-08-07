#include <iostream>
#include <string>
#include <sstream>
using namespace std;

void printDecAsHex(int n) {
    string temp="";
    if (n>0) {
        int mod=n%16;
        if (mod>9) {
            switch(mod){
                case 10:
                    temp.insert(temp.begin(), 'A');
                    break;
                case 11:
                    temp.insert(temp.begin(), 'B');
                    break;
                case 12:
                    temp.insert(temp.begin(), 'C');
                    break;
                case 13:
                    temp.insert(temp.begin(), 'D');
                    break;
                case 14:
                    temp.insert(temp.begin(), 'E');
                    break;
                case 15:
                    temp.insert(temp.begin(), 'F');
                    break;
            }
        } else {
            stringstream ss;
            ss << mod;
            temp.insert(temp.begin(), ss.str()[0]);
        }
        n/=16;
        printDecAsHex(n);
    }
    cout << temp;
}

void printDecAsUndec(int n) {
    string temp="";
    while (n>0) {
        int mod=n%11;
        if (mod==10) {
            temp.insert(temp.begin(), 'A');
        } else {
            stringstream ss;
            ss << mod;
            temp.insert(temp.begin(), ss.str()[0]);
        }
        n/=11;
    }
    cout << temp;
}

int main()
{
    int t;
    cin >> t;
    while (t--) {
        int number;
        cin >> number;
        printDecAsHex(number);
        cout << " ";
        printDecAsUndec(number);
        cout << endl;
    }
    return 0;
}
