#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    int t, n;
    cin >> t;
    while (t--) {
        cin >> n;
        int number;
        vector<int> arr;
        for (int i=0; i<n; i++) {
            cin >> number;
            arr.push_back(number);
        }
        sort(arr.begin(), arr.end());
        int max = *max_element(arr.begin(), arr.end());
        for (vector<int>::iterator it=arr.begin(), int i=0; it!=arr.end(); it++, i++) {
            cout << *it << endl;
            if (*it==max && it!=arr.end()) {
                cout << *it << " ";
            } else if (*it==max) {
                cout << *it << endl;
            }
        }


    }
    return 0;
}
