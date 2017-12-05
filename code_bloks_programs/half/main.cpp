#include <iostream>

using namespace std;

int main()
{
    int t;
    cin >> t;
    while (t--) {
        string row;
        cin >> row;
        for (int i=0; i<row.length()/2; i++) {
            cout << row[i];
        }
        cout << endl;
    }
    return 0;
}
