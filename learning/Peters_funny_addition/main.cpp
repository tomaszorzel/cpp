#include <iostream>
#include <sstream>
#include<bits/stdc++.h>
using namespace std;

string to_string(int i)
{
    stringstream ss;
    ss << i;
    return ss.str();
}

int makePolindrom(int);

int counter=0;

int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        int n;
        cin >> n;
        counter=0;
        cout << makePolindrom(n) << " ";
        cout << counter << endl;
    }
    return 0;
}

int makePolindrom(int n)
{
    string tempString = to_string(n);
    string nString = to_string(n);

    reverse(tempString.begin(), tempString.end());
    if (!tempString.compare(nString))
    {
        return n;
    }
    else
    {
        int tempStringAsInt = atoi(tempString.c_str());
        n += tempStringAsInt;
        counter++;
        return makePolindrom(n);
    }
}
