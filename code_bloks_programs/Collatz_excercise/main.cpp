#include <iostream>

using namespace std;

int n=0;

void collatz(int s) {
    if (s>1) {
        if (s%2>0) {
            n++;
            return collatz(3*s+1);
        } else {
            n++;
            return collatz(s/2);
        }
    }
}

int main()
{
    int t;
    cin >> t;
    while (t--) {
        int s;
        cin >> s;
        collatz(s);
        cout << n << endl;
        n=0;
    }
    return 0;
}
