#include <iostream>

using namespace std;

void f(int* p) {
    cout << *p << endl;
    *p=20;
    cout << *p << endl;
}

int main()
{
    int var=10;
    int* p=&var;
    f(p);
    cout << *p << endl;
    cout << var << endl;
    return 0;
}
