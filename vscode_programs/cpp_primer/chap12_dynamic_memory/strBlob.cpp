#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <iostream>

using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::initializer_list;
using std::cin;
using std::cout;
using std::endl;

class StrBlob {
public:
    typedef vector<string>::size_type size_type;
    StrBlob();
    StrBlob(initializer_list<string> il);
    size_type size() { return data->size(); }
    bool empty() { return data->empty(); }
    void push_back(const string &t) { data->push_back(t); }
    void pop_back();
    string& front();
    string& front() const;
    string& back();
    string& back() const;
private:
    shared_ptr<vector<string>> data;
    void check(size_type i, const string &msg) const;
};

StrBlob::StrBlob() : data(make_shared<vector<string>>()) {}
StrBlob::StrBlob(initializer_list<string> il) : data(make_shared<vector<string>>(il)) {}

void StrBlob::pop_back() {
    check(0, "pop_back on empty StrBlob");
    data->pop_back();
}

string& StrBlob::front() {
    check(0, "front on empty StrBlob");
    return data->front();
}

string& StrBlob::front() const {
    return front();
}

string& StrBlob::back() {
    check(0, "back on empty StrBlob");
    return data->back();
}

string& StrBlob::back() const {
    return back();
}

void StrBlob::check(size_type i, const string &msg) const {
    if (i >= data->size())
        throw std::out_of_range(msg);
}

#define SECTION_12_1_2
vector<int>* returnDynamicallyAllocatedVecOfInts() {
    vector<int> *pvi = new vector<int>;
    return pvi;
}

shared_ptr<vector<int>> returnDynamicallyAllocatedVecOfIntsShrdPtr() {
    return shared_ptr<vector<int>>();
}

void readCinToSetValuesInVec(vector<int> *pvi) {
    cout << "Give three values seperated by space: ";
    for (int i=0; i<3; ++i) {
        int temp;
        cin >> temp;
        pvi->push_back(temp);
    }
}

void printValuesInVec(vector<int> *pvi) {
    cout << "Values in vec: ";
    for (int i=0; i<pvi->size(); ++i ) {
        cout << pvi->at(i) << ( (i==(pvi->size()-1)) ? "\n" : " ");
    }
}

void practiceOnDynamicallyAllocatedVecNewDelete() {
    cout << "practiceOnDynamicallyAllocatedVecNewDelete():\n";
    vector<int> *pvi = returnDynamicallyAllocatedVecOfInts();
    readCinToSetValuesInVec(pvi);
    printValuesInVec(pvi);
    delete pvi;
}

void practiceOnDynamicallyAllocatedVecShrdPtr() {
    cout << "\npracticeOnDynamicallyAllocatedVecShrdPtr():\n";
    shared_ptr<vector<int>> shp = returnDynamicallyAllocatedVecOfIntsShrdPtr();
    readCinToSetValuesInVec();// continue here - how to pass shared_ptr to function that takes pointer
}

int main() {
    #ifdef SECTION_12_1_2
    practiceOnDynamicallyAllocatedVecNewDelete();
    practiceOnDynamicallyAllocatedVecShrdPtr();
    #endif // SECTION_12_1_2
    return 0;
}