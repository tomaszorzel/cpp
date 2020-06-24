#include <vector>
#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cstring>

using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::initializer_list;
using std::cin;
using std::cout;
using std::endl;

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

// #define SECTION_12_1_2
vector<int>* returnDynamicallyAllocatedVecOfInts() {
    vector<int> *pvi = new vector<int>;
    return pvi;
}

shared_ptr<vector<int>> returnDynamicallyAllocatedVecOfIntsShrdPtr() {
    return make_shared<vector<int>>();
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
    readCinToSetValuesInVec(shp.get());
    printValuesInVec(shp.get());
}

#define SECTION_12_1_3
void process(shared_ptr<int> ptr) {
    // doing sth with the ptr
}

void testProcess() {
    auto p = new int();
    auto sp = make_shared<int>();
    process(sp);
    // process(new int()); // error
    // process(p); // error
    process(shared_ptr<int>(p));
}

void deleteRawPtrOwnedBySmartPtrExplicitly() {
    auto sp = make_shared<int>();
    cout << "value in smart pointer: " << (sp.get()) << endl;
    // int *p = sp.get(); // error 
    // delete p; // error double free or corruption (out)
    // cout << "value in smart pointer after explicit delete of the pointer gotten from smart pointer: " << *(sp.get()) << endl;
}

#define SECTION_12_1_4
struct Destination {
    unsigned id;
    explicit Destination(unsigned destId) : id(destId) {}
};

struct Connection {
    Destination *dest;
    ~Connection() {
        cout << "Destruct connection object." << endl;
    }
};

Connection connect(Destination *dest) {
    Connection connection;
    cout << "Connecting to destination with id: " << dest->id << endl;
    connection.dest = dest;
    return connection;
}

void endConnection(Connection *conn) {
    conn->dest = nullptr;
    cout << "Pointer to destination object is cleaned-up. Now it points to " << conn->dest << ", which makes the address of destination is no longer occupied." << endl;
}

void f(Destination &dest) {
    cout << "f setups connection to given destination. Destination id: " << dest.id << endl;
    Connection connection = connect(&dest);
    shared_ptr<Connection> pc(&connection, endConnection);
    cout << "Check if connection to correct destination is setup via checking id." << endl;
    if (pc->dest->id == dest.id) {
        cout << "Connection for destination with id=" << dest.id << " setup successfully." << endl;
    } else {
        cout << "Connection for destination with id=" << dest.id << " setup failed." << endl;
    }

    // End the connection in f. f creates, does checking and ends the connection. Without shared_ptr I need to remember to call ennConnection
    // endConnection(&connection);
}

void f2(Destination &destination) {
    Connection newConnection = connect(&destination);
    shared_ptr<Connection> pointConnection(&newConnection, [](Connection* connectionToCleanUp){ connectionToCleanUp->dest=nullptr; });
}

void manageConnectionUsingSmartPointer() {
    cout << "\nmanageConnectionUsingSmartPointer():\n";    
    Destination destination(92);
    cout << "Call f to make operations on destination with id: " << destination.id << endl;
    f(destination);
    cout << "f finished it's operations." << endl;
}

void manageConnectionUsingSmartPointerAndUseLambda() {
    cout << "\nmanageConnectionUsingSmartPointerAndUseLambda():\n";
    Destination Destination(12);
    f2(Destination);
}

#define SECTION_12_1_5
void testCompileErrorsWhenCopyOrAssignUniquePtr() {
    cout << "\ntestCompileErrorsWhenCopyOrAssignUniquePtr():\n";
    std::unique_ptr<int> uptr1(new int(2));

    // Try to copy one unique_ptr to other which is error
    // std::unique_ptr<int> uptr2(uptr1);
    // std::unique_ptr<int> uptr3;
    // uptr3 = uptr1;
}

void checkIfCodeIsLegalOrNot() {
    cout << "\ncheckIfCodeIsLegalOrNot():\n";
    int x=1;
    int *p = &x;
    int *p2 = new int(1);
    delete p2;
    // std::unique_ptr<int> up(p);
    // cout << "*p=" << *p <<endl;
    // std::unique_ptr<int> up3(&x);
    // cout << "x=" << *up3 << endl;
    // std::unique_ptr<int> up2(p2);
    // std::unique_ptr<int> up4(up2.get());
}

#define SECTION_12_1_6
class StrBlobPtr {
public:
    StrBlobPtr() : curr(0) {}
    StrBlobPtr(StrBlob &a, size_t sz = 0) : wptr(a.data), curr(sz) {}
    std::string& deref() const;
    StrBlobPtr& incr();
    bool operator!=(const StrBlobPtr&) const;
private:
    std::shared_ptr<std::vector<std::string>> check(std::size_t, const std::string&) const;
    std::weak_ptr<std::vector<std::string>> wptr;
    std::size_t curr;
};

std::shared_ptr<std::vector<std::string>> StrBlobPtr::check(std::size_t sz, const std::string &msg) const {
    auto p = wptr.lock();
    if (!p) // there is no StrBlob object that wptr points to
        throw std::runtime_error("unbound StrBlobPtr");
    if (sz >= p->size())
        throw std::out_of_range(msg);
    return p;
}

std::string& StrBlobPtr::deref() const {
    // auto p = check(curr, "dereference past end");
    // return (*p)[curr];

    // alternatively:
    return ((*check(curr, "dereference past end"))[curr]);
}

StrBlobPtr& StrBlobPtr::incr() {
    check(curr, "increment past end of StrBlobPtr");
    curr++;
    return *this;
}

bool StrBlobPtr::operator!=(const StrBlobPtr &r) const {
    return curr != r.curr;
}

StrBlobPtr StrBlob::begin() {
    return StrBlobPtr(*this);
}

StrBlobPtr StrBlob::end() {
    return StrBlobPtr(*this, data->size());
}

void readLinesFromFileToStrBlobPrintDataUsingStrBlobPtr() {
    cout << "\nreadLinesFromFileToStrBlobPrintDataUsingStrBlobPtr():\n";
    std::string fileName = "inForStrBlobObj.txt";
    std::ifstream inFile(fileName);
    
    if (!inFile) {
        std::string msg = "File " + fileName + " not found!";
        throw std::runtime_error(msg);
    }

    std::string line;
    StrBlob sb;
    // while (getline(inFile, line)) {
    //     sb.push_back(line);
    // }
    // instead of while, for loop can be used also:
    for (; getline(inFile, line); sb.push_back(line)) {}

    inFile.close();

    StrBlobPtr sbptr(sb);
    for (StrBlobPtr beg=sb.begin(), end=sb.end(); beg != end; beg.incr())
        std::cout << beg.deref() << std::endl;
}

class ConstStrBlobPtr {
public:
    ConstStrBlobPtr() : curr(0) {}
    ConstStrBlobPtr(const StrBlob &a, size_t sz = 0) : wptr(a.data), curr(sz) {}
    const std::string& deref() const;
    ConstStrBlobPtr& incr();
    bool operator!=(const ConstStrBlobPtr&) const;
private:
    std::shared_ptr<std::vector<std::string>> check(std::size_t, const std::string&) const;
    std::weak_ptr<std::vector<std::string>> wptr;
    std::size_t curr;
};

const std::string& ConstStrBlobPtr::deref() const {
    const auto p = check(curr, "dereference past end");
    return (*p)[curr];
}

std::shared_ptr<std::vector<std::string>> ConstStrBlobPtr::check(std::size_t sz, const std::string &msg) const {
    auto p = wptr.lock();
    if (!p) // there is no StrBlob object that wptr points to
        throw std::runtime_error("unbound StrBlobPtr");
    if (sz >= p->size())
        throw std::out_of_range(msg);
    return p;
}

ConstStrBlobPtr& ConstStrBlobPtr::incr() {
    check(curr, "increment past end of StrBlobPtr");
    curr++;
    return (*this);
}

bool ConstStrBlobPtr::operator!=(const ConstStrBlobPtr &r) const {
    return curr != r.curr;
}

void createConstStrBlobPtrThatCanPointToConstStrBlob() {
    std::cout << "\ncreateConstStrBlobPtrThatCanPointToConstStrBlob():\n";
    // continue here (finish create constStrBlobPtr)
    StrBlob sb1({
        "Playing in empty stadiums is going to become the new norm for footballers, so it's probably best they start to get used to it.", 
        "That may have been the reasoning behind several Premier League teams holding training sessions at their grounds this week, with some even playing full friendlies to give their players a taste of what is to come.",
        "The Premier League is set to resume behind closed doors from 17 June after being suspended since 13 March because of the coronavirus pandemic."
        });

    ConstStrBlobPtr constpsb1(sb1);
    try {
        for (auto p=constpsb1; ; p.incr()) 
            std::cout << p.deref() << std::endl;
    } catch (std::out_of_range err) {
        std::cout << "StrBlob obj processing with ConstStrBlobPtr finished." << std::endl;
    } catch (std::runtime_error err) {
        std::cout << err.what() << std::endl;
    }
}

// #define SECTION_12_2_1
const char *concatenate(const char* l1, const char* l2, unsigned &sz, char* p) {
    unsigned i=0;
    for (; i<strlen(l1); ++i )
        p[i] = l1[i];

    for (int j=0; j<strlen(l2); ++j, ++i)
        p[i] = l2[j];
    p[sz-1] = '\0';
    return p;
}
void concatenateTwoStringLiteralsPutResultInDynamicallyAllocatedArray() {
    // 12.23 part 1
    std::cout << "\nconcatenateTwoStringLiteralsPutResultInDynamicallyAllocatedArray():\n";
    char literal1[] = "Hello";
    char literal2[] = " world";
    unsigned sz = strlen(literal1) + strlen(literal2)+1; // +1 needed for null sign at the end
    char *p = new char[sz];
    std::cout << "Concatenated strings (array of char): " << concatenate(literal1, literal2, sz, p) << endl;
    delete p;

    //12.23 part 2
    std::string s1(literal1), s2(literal2);
    std::string res = s1+s2;
    std::cout << "Concatenated strings (library string): " << res << endl;
}

void readStringFromStdInputIntoDynamicallyAllocatedArray() {
    std::string s;
    std::string sLongerThanOneFromUser = "longer";
    cin >> s;
    unsigned sizeToAllocate = s.size()+1;
    char* p = new char[sizeToAllocate];
    unsigned i=0;
    for(auto it=s.begin(); it!=s.end(); ++it, ++i) {
        p[i]=*it;
    }
    
    p[s.size()] = '\0';
    // p[s.size()+2] = 'a'; // writing outside of allocated memory works, but by this I am ruining memory that belongs to some other program! This is dangerous error!
    std::cout << "Read from std input and stored in dynamically allocated array: " << p << std::endl;
    delete p;
}

void practiceNewAndDelete() {
    std::cout << "\npracticeNewAndDelete()\n" << std::endl;
    int *pa = new int[10];
    delete [] pa;
}

class Dummy {
private:
    int number;
public:
    Dummy() : number(3) { std::cout << "CTOR for Dummy obj" << std::endl; }
    Dummy(int n) : number(n) { std::cout << "CTOR for Dummy obj" << std::endl; } 
    ~Dummy() { std::cout << "DTOR for Dummy obj" << std::endl; }
    int get() {
        return number;
    }
};

void checkWhenDTORUsedWhenMemoryIsDealocated() {
    std::cout << "\ncheckWhenDTORUsedWhenMemoryIsDealocated()\n";
    Dummy *p = new Dummy();
    std::cout << "p points to Dummy obj with number: " << p->get() << std::endl;
    delete p;

    Dummy *p2 = new Dummy(11);
    std::cout << "p2 points to Dummy obj with number: " << p2->get() << std::endl;
    delete p2;

    std::cout << "End of test" << std::endl;
}

// #define SECTION_12_2_2
void practiceAllocatorClass() {
    std::cout << "\npracticeAllocatorClass():\n";
    std::cout << "Provide n for size of string array: ";
    int n; 
    std::cin >> n;
    std::allocator<std::string> alloc;
    auto const p = alloc.allocate(n);
    std::string *q = p;
    std::string s;
    while (q != p+n && cin >> s) {
        alloc.construct(q++, s); // allocator class seperates allocation and construction. To have an obj in allocated memory we need to construct it there explicitly
    }
    const size_t size = q-p; // to remember how much of the allocated memory is actually used

    auto beg = p;
    auto end = p+size;
    for (; beg!=end; beg++)
        std::cout << *beg << endl;

    // destroy and deallocated memory
    alloc.destroy(p);
    alloc.deallocate(p, n);
}

//#define SECTION_12_3_1
// text query program

int main() {
    #ifdef SECTION_12_1_2
    practiceOnDynamicallyAllocatedVecNewDelete();
    practiceOnDynamicallyAllocatedVecShrdPtr();
    #endif // SECTION_12_1_2

    #ifdef SECTION_12_1_3
    testProcess();
    deleteRawPtrOwnedBySmartPtrExplicitly();
    #endif // SECTION_12_1_3

    #ifdef SECTION_12_1_4
    manageConnectionUsingSmartPointer();
    manageConnectionUsingSmartPointerAndUseLambda();
    #endif // SECTION_12_1_4

    #ifdef SECTION_12_1_5
    testCompileErrorsWhenCopyOrAssignUniquePtr();
    checkIfCodeIsLegalOrNot();
    #endif // SECTION_12_1_5

    #ifdef SECTION_12_1_6
    readLinesFromFileToStrBlobPrintDataUsingStrBlobPtr();
    createConstStrBlobPtrThatCanPointToConstStrBlob();
    #endif

    #ifdef SECTION_12_2_1
    concatenateTwoStringLiteralsPutResultInDynamicallyAllocatedArray();
    readStringFromStdInputIntoDynamicallyAllocatedArray();
    practiceNewAndDelete();
    #endif

    checkWhenDTORUsedWhenMemoryIsDealocated();

    #ifdef SECTION_12_2_2
    practiceAllocatorClass();
    #endif

    #ifdef SECTION_12_3_1
    findOccurencesInInputFile();
    #endif

    return 0;
}