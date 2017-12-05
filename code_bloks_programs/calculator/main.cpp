#include <iostream>
using namespace std;

bool isOperationAcceptable(char sign) {
    if (sign=='+' || sign=='-' || sign=='*' || sign=='/' || sign=='%')
        return true;
    else
        return false;
}

int main() {

    char sign;
    int a, b, counter=0;
    while (cin >> sign >> a >> b && isOperationAcceptable(sign)) {
        switch (sign) {
        case '+':
            cout << a+b << endl;
            break;
        case '-':
            cout << a-b << endl;
            break;
        case '*':
            cout << a*b << endl;
            break;
        case '/':
            cout << a/b << endl; // Dividing by 0 is not considered in this excercise.
                                 // It is also enough to print result as an int.
            break;
        case '%':
            cout << a%b << endl;
            break;
        default:
            break;
        }
    }
    return 0;
}
