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

        for (int i=1; i<n; ) {
            if (i==1) cout << arr[i];
            else cout << " " << arr[i];
            i+=2;
        }

        for (int i=0; i<n; ) {
            cout << " " << arr[i];
            i+=2;
        }
        cout << endl;
        delete [] arr;
    }
    return 0;
}
