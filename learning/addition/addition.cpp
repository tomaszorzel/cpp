#include <iostream>

using namespace std;

int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        int size;
        cin >> size;

        int *valuesToAdd = new int[size];
        int result=0;
        for (int i=0; i<size; i++)
        {
            cin >> valuesToAdd[i];
            result+=valuesToAdd[i];
        }
        cout << result << endl;

        delete [] valuesToAdd;
    }
    return 0;
}
