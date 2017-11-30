#include <iostream>
#include <string>
using namespace std;

int main()
{
    string numbers;
    cin >> numbers;
    int length = numbers.size(); cout << "length=" << length << endl;
    int dynArrLen=(length+1)/2; cout << "dynArrLen=" << dynArrLen << endl;
    int* dynArr = new int[dynArrLen];

    for (int i=0, j=0; i<dynArrLen; i++, j+=2) {
        dynArr[i]=numbers[j];
        cout << numbers [j] << endl;
    }

    for (int i=0; i<dynArrLen; i++) {
        cout << dynArr[i] << endl;
    }
    cout << endl;
    return 0;
}
