#include <iostream>

using namespace std;

int main()
{
    int stack[10]={0,0,0,0,0,0,0,0,0,0};
    int index=-1;
    int enteredNumber;
    char selectionOfCommand;
    while (cin >> selectionOfCommand) {
        if (selectionOfCommand=='+') {
            cin >> enteredNumber;
            if (index<9 && index>=-1) {
                stack[++index]=enteredNumber;
                cout << ":)" << endl;
            } else {
                cout << ":(" << endl;
            }
        } else if (selectionOfCommand=='-') {
            if (index==10) {
                cout << stack[--index] << endl;
                index--;
            }
            else if (index>=0)
                cout << stack[index--] << endl;
            else if (index<0)
                cout << ":(" << endl;
        } else
            break;
    }
    return 0;
}
