#include <iostream>

using namespace std;

bool isOperationAcceptable(const char sign) {
    if (sign=='+' || sign=='-' || sign=='*' || sign=='/' || sign=='%' || sign=='z')
        return true;
    else
        return false;
}

int main()
{
    char sign;
    int a, b, registers[10]={0,0,0,0,0,0,0,0,0,0}, index=0;
    while (cin >> sign >> a >> b && isOperationAcceptable(sign)) {
        switch (sign) {
        case '+':
            cout << registers[a]+registers[b] << endl;
            break;
        case '-':
            cout << registers[a]-registers[b] << endl;
            break;
        case '*':
            cout << registers[a]*registers[b] << endl;
            break;
        case '/':
            cout << registers[a]/registers[b] << endl; // It is enough to print result as an int.
            break;
        case '%':
            cout << registers[a]%registers[b] << endl;
            break;
        case 'z':
            index=a;
            registers[index]=b;
            break;
        }
    }
    return 0;
}
