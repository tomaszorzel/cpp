#include <iostream>
using namespace std;

 int main()
 {
 	int t;
 	cin>>t;
 	while (t--) {
 		int noOfStudents, noOfCandies;
 		cin>>noOfStudents>>noOfCandies;
 		if (--noOfStudents<=0)
 			cout<<"TAK"<<endl;
 		else {
 			int candiesLeft=noOfCandies%noOfStudents;
 			if (candiesLeft)
 				cout<<"TAK"<<endl;
 			else
 				cout<<"NIE"<<endl;
 		}
 	}
 	return 0;
 }
