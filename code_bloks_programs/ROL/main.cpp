#include <iostream>

using namespace std;

int main()
{
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        int* arr = new int[n];

        for (int i=0; i<n; i++) {
            cin >> arr[i];
        }

        int temp = arr[0];
        for (int i=0; i<n-1; i++) {
            arr[i]=arr[i+1];
        }
        arr[n-1]=temp;

        for (int i=0; i<n; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;

        delete [] arr;
    }
    return 0;
}
