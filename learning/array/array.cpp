#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int n;
    vector<int> providedNumbers;
    while (cin>>n) {
        providedNumbers.push_back(n);
    }
    for(vector<int>::iterator it=providedNumbers.end()-1;
            it!=providedNumbers.begin(); it--) {
        cout<<*it<<" ";
    }
    cout<<*providedNumbers.begin();
    return 0;
}
