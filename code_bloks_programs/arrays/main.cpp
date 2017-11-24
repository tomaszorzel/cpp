#include <iostream>

using namespace std;

int main() {
    int t;
    cin >> t;
    while (t--)
    {
        int n;
        cin >> n;
        int* arr = new int[n];
        for (int i=0; i<n; i++)
        {
            cin >> arr[i];
        }

        for (int i=0; i<n/2; i++)
        {
            int temp = arr[i];
            arr[i] = arr[n-1-i];
            arr[n-1-i] = temp;
        }

        for (int i=0; i<n; i++)
        {
            if (i==(n-1))
                cout << arr[i] << endl;
            else
                cout << arr[i] << " ";
        }
        delete [] arr;
    }
    return 0;
}
