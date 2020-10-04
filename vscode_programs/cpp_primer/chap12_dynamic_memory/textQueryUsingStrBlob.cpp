#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

class StrBlobPtr;
class ConstStrBlobPtr;
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
    friend class StrBlobPtr;
    friend class ConstStrBlobPtr;
    StrBlobPtr begin();
    StrBlobPtr end();
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

int main () {

    return 0;
}