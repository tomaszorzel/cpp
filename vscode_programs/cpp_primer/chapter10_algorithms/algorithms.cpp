#include <iostream>
#include <algorithm> // std::equal, std::fill_n
#include <vector>
#include <list>
#include <numeric> // std::accumulate
#include <cstring>
#include <iterator> // back_inserter

using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::list;
using std::string;
using std::accumulate;
using std::equal;
using std::fill_n;

template<typename T>
void printElemsInSeqContainerWithSpaceBetween(T t) {
    for (auto elem : t)
        cout << elem << " ";
}

#define SECTION_101
void useCountAlgorithmForVecOfInts() {
    cout << "useCountAlgorithmForVecOfInts():" << endl;
    vector<int> vi = {1,2,3,4,5,1,3,4,5,6,7,3,5,32,6};
    int counter = count(vi.begin(), vi.end(), 1);
    cout << "Counter: " << counter << endl;
}

void useCountAlgorithmForListOfStrings() {
    cout << "\nuseCountAlgorithmForListOfStrings()\n";
    list<string> ls = {"stringA", "stringB", "stringC", "stringA", "stringD"};
    int counter = count(ls.begin(), ls.end(), "stringA");
    cout << "Counter: " << counter << endl;
}

#define SECTION_10_2_1
void accumulateSumOfIntsInVec() {
    cout << "\naccumulateSumOfIntsInVec\n";
    vector<int> vi = {100, 200, 300, 333};
    int sum = accumulate(vi.cbegin(), vi.cend(), 0);
    cout << "Sum of elements in vi using accumulate: " << sum << endl;

    vector<double> vd = {3.14, 2.00, 4.02};
    double sumd = accumulate(vd.cbegin(), vd.cend(), double(0));
    cout << "Sum of elements in vd using accumulate: " << sumd << endl;

    const char* roster1 = "Have a nice day!"; 
    const char* roster2 = "Have a nice day!";
    
    cout << "Call equal on two cstrings: " << equal(roster1, roster1+strlen(roster1), roster2) << endl;
}

// #define SECTION_10_2_2
void useFillNToSetSeqOfInts() {
    cout << "\nuseFillNToSetSeqOfInts\n";
    vector<int> vi = {0,1,2,3,4,5,6,7,8,9};
    fill_n(vi.begin(), 10, 0);
    cout << "Vector after fill_n call: ";
    printElemsInSeqContainerWithSpaceBetween(vi); cout << endl;
    fill_n(std::back_inserter(vi), 10, 1);
    cout << "Vector after fill_n call with back_inserter used: ";
    printElemsInSeqContainerWithSpaceBetween(vi); cout << endl;
}

void correctErrorsSection10_2_2() {
    vector<int> vec;
    list<int> lst;
    int i;
    while (cin >> i)
        lst.push_back(i);
    copy(lst.begin(), lst.end(), std::back_inserter(vec));
    cout << "vec after copying: ";
    printElemsInSeqContainerWithSpaceBetween(vec); cout << endl;
}

int main() {

    #ifdef SECTION_101
    useCountAlgorithmForVecOfInts();
    useCountAlgorithmForListOfStrings();
    #endif // SECTION_101

    #ifdef SECTION_10_2_1
    accumulateSumOfIntsInVec();
    #endif // SECTION_10_2_1

    #ifdef SECTION_10_2_2
    useFillNToSetSeqOfInts();
    correctErrorsSection10_2_2();
    #endif // SECTION_10_2_2

    return 0;
}