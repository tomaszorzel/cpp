#include <iostream>

using namespace std;

int main()
{
    int a, counter=0, temp=42;
    while (cin>>a) {
        cout << a << endl;
        if (temp!=42 && a==42) {
            counter++;
        }
        temp=a;
        if (counter==3)
            break;
    }
    return 0;
}
