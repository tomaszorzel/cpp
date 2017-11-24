// C++ program to print largest contiguous array sum
#include<iostream>
using namespace std;

int main()
{
    long long sum=0, maxSumSoFar=0;
    int noOfCities;
    cin >> noOfCities;
    while (noOfCities--) {
        int gainAtConcert;
        cin >> gainAtConcert;
        sum+=gainAtConcert;
        if (sum>maxSumSoFar) maxSumSoFar=sum;
        if (sum<0) sum=0;
    }
    cout << maxSumSoFar << endl;

    return 0;
}
