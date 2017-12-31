#include <iostream>
using namespace std;

void swipeByK(int* digits, int noOfDigits, int k) {
    int* tempArr=new int[k];
    for(int i=0; i<k; i++) {
        tempArr[i]=digits[i];
    }

    for(int i=0; i<(noOfDigits-k); i++) {
        digits[i]=digits[i+k];
    }

    for(int i=0,j=k; i<k; i++,j--) {
        digits[noOfDigits-j]=tempArr[i];
    }
    delete [] tempArr;
}

int main() {
    int noOfDigits, k;
 	cin>>noOfDigits>>k;
    if (noOfDigits>0) {
        int* digits=new int[noOfDigits];

        for(int i=0; i<noOfDigits; i++) {
            cin>>digits[i];
        }

        swipeByK(digits, noOfDigits, k);

        cout<<digits[0];
        for(int i=1; i<noOfDigits; i++) {
            cout<<" "<<digits[i];
        }
        cout<<endl;

        delete [] digits;
    }

 	return 0;
}
