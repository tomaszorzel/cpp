#include <iostream>
#include <cmath>
#include <cfloat>
using namespace std;

int main()
{
    int t;
    cin >> t;
    while (t--) {
        int n, result;
        double sum=0.0, average=0.0;
        double diff=0.0, temp=DBL_MAX;
        cin >> n;
        int* arr = new int[n];
        for (int i=0; i<n; i++) {
            cin >> arr[i];
            sum+=arr[i];
        }
        average=(double)sum/n;
        for (int i=0; i<n; i++) {
            diff=arr[i]-average;
            diff=abs(diff);
            if (diff<temp) {
                result=arr[i];
                temp=diff;
            }
        }
        cout << result << endl;
        delete [] arr;
    }
    return 0;
}
