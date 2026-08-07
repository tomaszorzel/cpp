#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    int t, howManyNumbers;
    cin >> t;
    while (t--) {
        cin >> howManyNumbers;
        int number;
        vector<int> arr;
        for (int i=0; i<howManyNumbers; i++) {
            cin >> number;
            arr.push_back(number);
        }
        sort(arr.begin(), arr.end());
        int max = *max_element(arr.begin(), arr.end());
        int counter=0;
        for (unsigned i=0; i<arr.size(); i++) {
            if (arr.at(i)==max) {
                if (counter==arr.size()-1)
                    cout << arr.at(i);
                else
                    cout << arr.at(i) << " ";
                counter++;
            }
        }

        for (unsigned i=0; i<arr.size(); i++) {
            if (arr.at(i)!=max) {
                if (counter<arr.size())
                    cout << arr.at(i) << " ";
                else
                    cout << arr.at(i);
            }
        }
        cout << endl;
    }
    return 0;
}
