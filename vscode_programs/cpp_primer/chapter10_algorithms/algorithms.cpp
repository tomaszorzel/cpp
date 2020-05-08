#include <iostream>
#include <algorithm> // std::equal, std::fill_n, std::partition
#include <vector>
#include <list>
#include <deque>
#include <numeric> // std::accumulate
#include <cstring>
#include <iterator> // back_inserter
#include <functional> //bind
#include <fstream> // ifstream

using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::list;
using std::string;
using std::accumulate;
using std::equal;
using std::fill_n;
using std::placeholders::_1;
using std::deque;

template<typename T>
void printElemsInSeqContainerWithSpaceBetween(T t) {
    for (auto elem : t)
        cout << elem << " ";
}

// #define SECTION_101
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

// #define SECTION_10_2_1
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

#define SECTION_10_2_3
void elimDups(vector<string> &); // declaration
void elimDups() {
    cout << "\nelimDups():\n";
    vector<string> vs = {"hello", "it", "is", "me", "how", "you", "are", "doing", "is", "hello", "how", "you"};
    elimDups(vs);
}

void elimDups(vector<string> &vs) {
    cout << "Vector: ";
    printElemsInSeqContainerWithSpaceBetween(vs); cout << endl;
    sort(vs.begin(), vs.end());
    cout << "Vector sorted: ";
    printElemsInSeqContainerWithSpaceBetween(vs); cout << endl;
    auto end_unique = unique(vs.begin(), vs.end());
    cout << "Vector after calling unique: ";
    printElemsInSeqContainerWithSpaceBetween(vs); cout << endl;
    vs.erase(end_unique, vs.end());
    cout << "Vector after calling erase: ";
    printElemsInSeqContainerWithSpaceBetween(vs); cout << endl;
}

#define SECTION_10_3_1
bool isShorter(const string &str1, const string &str2) {
    return (str1.size() < str2.size());
}

void useStableSortAlgorithm() {
    cout << "\nuseStableSortAlgorithm():\n";
    vector<string> vs = {"hello", "it", "is", "me", "how", "you", "are", "doing", "is", "hello", "how", "you"};
    elimDups(vs);
    stable_sort(vs.begin(), vs.end(), isShorter);
    cout << "Vector after sorting it also by the length: ";
    printElemsInSeqContainerWithSpaceBetween(vs); cout << endl;
}

#include "book_store.h"
bool compareIsbn(const Sales_data &book1, const Sales_data &book2) {
    if (book1.isbn().compare(book2.isbn()) < 0 )
        return true;
    else
        return false;
}

void sortSalesDataObject() {
    cout << "\nsortSalesDataObject():\n";
    Sales_data sd1("999-83-999999-9-9", 0, 0);
    Sales_data sd2("950-83-999999-9-9", 0, 0);
    Sales_data sd3("930-83-999999-9-9", 0, 0);
    Sales_data sd4("999-83-999999-9-9", 0, 0);
    vector<Sales_data> vsd = {sd1, sd2, sd3, sd4};
    cout << "Vector before sorting Sales_data elements: ";
    for (auto sd : vsd)
        cout << sd.isbn() << " ";
    cout << endl;
    cout << "Vector after sorting Sales_data elements: ";
    // sort(vsd.begin(), vsd.end(), compareIsbn); // below same sorting but using lambda expression
    sort(vsd.begin(), vsd.end(), [](const Sales_data &book1, const Sales_data &book2)
                                    {
                                        if (book1.isbn().compare(book2.isbn()) < 0)
                                            return true;
                                        else
                                            return false; 
                                    }
    );
    for (auto sd : vsd)
        cout << sd.isbn() << " ";
    cout << endl;
}

bool isOfLengthOfFiveOrMore(string s) {
    return (s.length() >= 5);
}

void partitionVecOfStringsDependingOnLengthOfTheElement() {
    cout << "\npartitionVecOfStringsDependingOnLengthOfTheElement(int):\n";
    vector<string> vs = {"nomeaning", "Hello", "Zinedine", "what", "is", "your", "day", "let", "be", "more", "thoughtful"};
    cout << "Vector before partitioning: ";
    printElemsInSeqContainerWithSpaceBetween(vs); cout << endl;
    vector<string>::iterator bound = std::partition(vs.begin(), vs.end(), isOfLengthOfFiveOrMore);
    cout << "Elements in vector that have length equal or greater than 5: ";
    vector<string>::iterator it = vs.begin();
    while ( it != bound) {
        cout << *(it++) << " ";
    }
    cout << endl;
    cout << "And a whole vector: ";
    printElemsInSeqContainerWithSpaceBetween(vs); cout << endl;
}

#define SECTION_10_3_2
void useLambdaToSumTwoInts() {
    cout << "\nuseLambdaToSumTwoInts():\n";
    auto sum =  [](const int &a, const int &b) -> int {
        return a+b;
    };
    cout << "Sum two ints using lambda: " << sum(55, 35) << endl;
}

void useLambdaToSumTwoInts2() {
    cout << "\nuseLambdaToSumTwoInts2():\n";
    int a = 55;
    auto sum = [a](const int &b) -> int {
        return a+b;
    };
    cout << "Sum two ints using lambda2: " << sum(45) << endl;
}

void biggies() {
    cout << "\nbiggies():\n";
    vector<string> vs = 
    {
        "the", "government", "plans", "to", "set", "up", "the", "first", "of", "a", "series", "of", "regular",
        "meetings", "involving", "senior", "medical", "directors", "of", "the", "major", "sports", "this",
        "week", "in", "a", "bid", "to", "return", "to", "action", "as", "soon", "as", "possible"
    };

    cout << "Vector before sorting: ";
    printElemsInSeqContainerWithSpaceBetween(vs); cout << endl;
    sort(vs.begin(), vs.end());
    cout << "Vector after sorting: ";
    printElemsInSeqContainerWithSpaceBetween(vs); cout << endl;

    // Lambda expression:
    auto isShorter = [] (const string &s1, const string &s2) -> bool {
        // cout << "// Lambda is shorter called" << endl;
        return s1.size() < s2.size();
    };

    stable_sort(vs.begin(), vs.end(), isShorter);
    cout << "Vector after stable sorting: ";
    printElemsInSeqContainerWithSpaceBetween(vs); cout << endl;

    // Eliminate duplicates
    auto firstNonUniqueWord = unique(vs.begin(), vs.end());
    cout << "Vector after moving non unique words to the end of it: ";
    printElemsInSeqContainerWithSpaceBetween(vs); cout << endl << "Size is: " << vs.size() << endl;
    vs.erase(firstNonUniqueWord, vs.end());
    cout << "Vector after erasing non unique words: ";
    printElemsInSeqContainerWithSpaceBetween(vs); cout << endl << "Size is: " << vs.size() << endl;

    // Print only elements of given size or longer
    cout << "Print only elements of given size (5) or longer: ";
    string::size_type givenSize = 5;
    // use find_if + lambda expr
    // auto firstElemOfGivenSize = find_if( vs.begin(), vs.end(), [givenSize](const string &s) { return s.size() >= givenSize; } );
    // use partition or stable_partition + lambda expr
    auto firstElemOfGivenSize = stable_partition(vs.begin(), vs.end(), [givenSize](const string &s) { return s.size() < givenSize; } );
    for (auto it = firstElemOfGivenSize; it!=vs.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
    cout << "Current vector arrangement: ";
    printElemsInSeqContainerWithSpaceBetween(vs); cout << endl;
}

#define SECTION_10_3_3
void countHowManyWordsHaveLengthOfGivenValue() {
    cout << "\ncountHowManyWordsHaveLengthOfGivenValue():\n";
    vector<string> vs = 
    {
        "the", "government", "plans", "to", "set", "up", "the", "first", "of", "a", "series", "of", "regular",
        "meetings", "involving", "senior", "medical", "directors", "of", "the", "major", "sports", "this",
        "week", "in", "a", "bid", "to", "return", "to", "action", "as", "soon", "as", "possible"
    };
    cout << "Vector: ";
    printElemsInSeqContainerWithSpaceBetween(vs); cout << endl;

    string::size_type size = 6;
    auto howMany = count_if(vs.begin(), vs.end(), [size](const string &s){ return s.size()>=6; });
    cout << "Number of words with length of " << size << " or more in vector: " << howMany << endl;
}

void useLambdaExprToDecrementIntTillZero() {
    cout << "\nuseLambdaExprToDecrementIntTillZero()\n";
    int n = 14;
    cout << "n before decrementation with lambda expr: " << n << endl;
    auto decrement = [&n]() -> bool {
        while (n!=0) {
            --n;
            // cout << "decremented ";
        }
        // cout << endl;
        return n==0;
    };
    cout << "n decremented?: " << decrement() <<  ". Value of n: " << n << endl;
}

#define SECTION_10_3_4
bool checkSize(const string &s, const int size) {
    return s.size() >= size;
}

bool checkSize2(const string &s, const int size) {
    return s.size() < size;
}

void useBindFuncInPlaceOfLambda() {
    cout << "\nuseBindFuncInPlaceOfLambda():\n";
    vector<string> vs = 
    {
        "the", "government", "plans", "to", "set", "up", "the", "first", "of", "a", "series", "of", "regular",
        "meetings", "involving", "senior", "medical", "directors", "of", "the", "major", "sports", "this",
        "week", "in", "a", "bid", "to", "return", "to", "action", "as", "soon", "as", "possible"
    };
    cout << "Vector: ";
    printElemsInSeqContainerWithSpaceBetween(vs); cout << endl;
    string::size_type size = 6;
    
    auto howMany = count_if(vs.begin(), vs.end(), bind(checkSize, _1, size));
    cout << "Number of words with length of " << size << " or more in vector: " << howMany << endl;
}

void useBindFuncToFindFirstIntInVecThatHasValueGreaterThanGivenString() {
    cout << "\nuseBindFuncToFindFirstIntInVecThatHasValueGreaterThanGivenString():\n";
    string controlString = "control"; // length of 7
    vector<int> vi = {1,2,3,4,5,6,7,7,4,8};
    cout << "Vector of ints: ";
    printElemsInSeqContainerWithSpaceBetween(vi); cout << endl;
    cout << "Length of \"" << controlString << "\" is: " << controlString.length() << endl;
    auto firstValueGreaterThanLengthOfGivenString = find_if(vi.begin(), vi.end(), bind(checkSize2, cref(controlString), _1));
    cout << "First value in vector that is greater than length of given string is: " << *firstValueGreaterThanLengthOfGivenString << endl;
}

void biggies2() {
    cout << "\nbiggies2():\n";
    vector<string> vs = 
    {
        "the", "government", "plans", "to", "set", "up", "the", "first", "of", "a", "series", "of", "regular",
        "meetings", "involving", "senior", "medical", "directors", "of", "the", "major", "sports", "this",
        "week", "in", "a", "bid", "to", "return", "to", "action", "as", "soon", "as", "possible"
    };

    cout << "Vector before sorting: ";
    printElemsInSeqContainerWithSpaceBetween(vs); cout << endl;
    sort(vs.begin(), vs.end());
    cout << "Vector after sorting: ";
    printElemsInSeqContainerWithSpaceBetween(vs); cout << endl;

    // Lambda expression:
    auto isShorter = [] (const string &s1, const string &s2) -> bool {
        // cout << "// Lambda is shorter called" << endl;
        return s1.size() < s2.size();
    };

    stable_sort(vs.begin(), vs.end(), isShorter);
    cout << "Vector after stable sorting: ";
    printElemsInSeqContainerWithSpaceBetween(vs); cout << endl;

    // Eliminate duplicates
    auto firstNonUniqueWord = unique(vs.begin(), vs.end());
    cout << "Vector after moving non unique words to the end of it: ";
    printElemsInSeqContainerWithSpaceBetween(vs); cout << endl << "Size is: " << vs.size() << endl;
    vs.erase(firstNonUniqueWord, vs.end());
    cout << "Vector after erasing non unique words: ";
    printElemsInSeqContainerWithSpaceBetween(vs); cout << endl << "Size is: " << vs.size() << endl;

    // Print only elements of given size or longer
    cout << "Print only elements of given size (5) or longer: ";
    string::size_type givenSize = 5;
    // use find_if + lambda expr
    // auto firstElemOfGivenSize = find_if( vs.begin(), vs.end(), [givenSize](const string &s) { return s.size() >= givenSize; } );
    // use partition or stable_partition + lambda expr
    // auto firstElemOfGivenSize = stable_partition(vs.begin(), vs.end(), [givenSize](const string &s) { return s.size() < givenSize; } );
    auto firstElemOfGivenSize = stable_partition(vs.begin(), vs.end(), bind(checkSize2, _1, givenSize));
    for (auto it = firstElemOfGivenSize; it!=vs.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
    cout << "Current vector arrangement: ";
    printElemsInSeqContainerWithSpaceBetween(vs); cout << endl;
}

#define SECTION_10_4_1
void uniqeCopyToInitiallyEmptyList(){
    cout << "\nuniqeCopyToInitiallyEmptyList():\n";
    list<int> li = {1,1,1,2,2,3,4,3,5,6,7,8,9,10,1,3,4,5,6,7,8,8};
    cout << "List1: ";
    printElemsInSeqContainerWithSpaceBetween(li); cout << endl;
    list<int> li2; // empty list
    unique_copy(li.begin(), li.end(), inserter(li2, li2.begin()));
    cout << "List2 after using unique_copy to fill it: ";
    printElemsInSeqContainerWithSpaceBetween(li2); cout << endl;
}

void use3InsertersToFillEmptyVecs() {
    cout << "\nuse3InsertersToFillEmptyVecs():\n";
    vector<int> vi = {1,2,3,4,5,6,7,8,9};
    deque<int> di;
    vector<int> vi3, vi4; // empty vecs
    cout << "vi: "; printElemsInSeqContainerWithSpaceBetween(vi); cout << endl;
    cout << "Test front_inserter for di; di: ";
    copy(vi.begin(), vi.end(), front_inserter(di));
    printElemsInSeqContainerWithSpaceBetween(di); cout << endl;
    cout << "Test inserter for vi3; vi3: ";
    copy(vi.begin(), vi.end(), inserter(vi3, vi3.begin()));
    printElemsInSeqContainerWithSpaceBetween(vi3); cout << endl;
    cout << "Test back_inserter for vi4; vi4: ";
    copy(vi.begin(), vi.end(), back_inserter(vi4));
    printElemsInSeqContainerWithSpaceBetween(vi4); cout << endl;
}

#define SECTION_10_4_2
void useStreamIteratorToReadFile() {
    cout << "\nuseStreamIteratorToReadFile():\n";
    std::ifstream inputFile("sport_news_05_05_2020.txt");
    vector<string> vs;
    if (inputFile) {
        std::istream_iterator<string> in(inputFile), eof;
        vs = vector<string>(in, eof);
    } else {
        cout << "Failed to open the file" << endl;
    }
    cout << "Vector read from file with stream iterator: size: " << vs.size() << ", content: ";
    printElemsInSeqContainerWithSpaceBetween(vs); cout << endl;
}

void useStreamIteratorToReadIntsFromStdInSortAndPrintOnStdOut() {
    cout << "\nuseStreamIteratorToReadIntsFromStdInSortAndPrintOnStdOut():\n";
    std::istream_iterator<int> in_iter(cin), eof; 
    vector<int> vi(in_iter, eof); 
    sort(vi.begin(), vi.end());
    std::ostream_iterator<int> out_iter(cout, " ");
    cout << "Vector of sorted ints read from std input: ";
    copy(vi.begin(), vi.end(), out_iter);
    cout << endl;
    cout << "Vector of sorted and unique ints read from std input: ";
    unique_copy(vi.begin(), vi.end(), out_iter);
    cout << endl;
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

    #ifdef SECTION_10_2_3
    elimDups();
    #endif // SECTION_10_2_3

    #ifdef SECTION_10_3_1
    useStableSortAlgorithm();
    sortSalesDataObject();
    partitionVecOfStringsDependingOnLengthOfTheElement();
    #endif // SECTION_10_3_1

    #ifdef SECTION_10_3_2
    useLambdaToSumTwoInts();
    useLambdaToSumTwoInts2();
    biggies(); // Having vector of strings, eliminate duplicates and sort them. Then print only those that are longer or equal to given size.
    #endif // SECTION_10_3_2

    #ifdef SECTION_10_3_3
    countHowManyWordsHaveLengthOfGivenValue();
    useLambdaExprToDecrementIntTillZero();
    #endif // SECTION_10_3_3

    #ifdef SECTION_10_3_4
    useBindFuncInPlaceOfLambda();
    useBindFuncToFindFirstIntInVecThatHasValueGreaterThanGivenString();
    biggies2();
    #endif // SECTION_10_3_4

    #ifdef SECTION_10_4_1
    uniqeCopyToInitiallyEmptyList();
    use3InsertersToFillEmptyVecs();
    #endif // SECTION_10_4_1

    #ifdef SECTION_10_4_2
    useStreamIteratorToReadFile();
    useStreamIteratorToReadIntsFromStdInSortAndPrintOnStdOut();
    #endif // SECTION_10_4_2

    return 0;
}