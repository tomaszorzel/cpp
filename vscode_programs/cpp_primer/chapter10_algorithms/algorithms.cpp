#include <iostream>
#include <algorithm>
#include <vector>
#include <list>

using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::list;
using std::string;

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

int main() {

    #ifdef SECTION_101
    useCountAlgorithmForVecOfInts();
    useCountAlgorithmForListOfStrings();
    #endif // SECTION_101

    return 0;
}