#define ANSI
#include <iostream>
#include <stdio.h>
#include <iomanip>
using namespace std;

void printfloat(float);

int main()
{
    int t;
    float x;
    cin >> t; /* wczytaj liczbe testow */
    while(t)
    {
        cin >> x;
        printfloat(x);
        t--;
    }
    return 0;
}

void printfloat(float x)
{
    for (int i = sizeof x-1; i>=0; --i)
    {
        printf("%x ",(((const unsigned char*)&x)[i]));
    }
}
