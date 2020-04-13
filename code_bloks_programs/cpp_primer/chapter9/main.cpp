#include <iostream>
#include <vector>
#include <list>
#include <forward_list>
#include <deque>
#include <map>
using namespace std;

vector<int>::iterator isInVec(vector<int>::iterator beg, vector<int>::iterator end, int number) {
  while (beg != end) {
    if (*beg == number)
      return beg;
    beg++;
  }
  return end;
}

bool compareVecI(vector<int> vi1, vector<int> vi2) {
  return vi1 == vi2;
}

bool compareVecILI(vector<int> vi, list<int> li) {
  if (vi.size() != li.size())
    return false;

  bool result = true;
  vector<int>::iterator vibeg=vi.begin();
  list<int>::iterator libeg=li.begin();
  for (; vibeg != vi.end(); ++vibeg, ++libeg) {
    if (*vibeg != *libeg)
      return false;
  }
  return result;
}


void insertStringToFwdLst(forward_list<string> &fwdLst, string insertAfter, string toInsert) {
  bool found = false;
  auto curr = fwdLst.begin();
  auto prev = fwdLst.before_begin();
  while (curr != fwdLst.end()) {
    if ( *curr == insertAfter ) {
      curr = fwdLst.insert_after(curr, toInsert);
      found = true;
    } else {
      prev = curr;
      ++curr;
    }
  }

  if (!found)
    curr = fwdLst.insert_after(prev, toInsert);
}

void removeEvenAndDoubleOddInFwdList( /*forward_list<int> &fwdList*/ ) {
  forward_list<int> fwdList = {0,1,2,3,4,5,6,7,8,9};
  auto prev = fwdList.before_begin();
  auto curr = fwdList.begin();
  while ( curr != fwdList.end() ) {
    if (*curr%2) {
      prev = fwdList.insert_after(prev, *curr);
      curr++;
      prev++;
    } else {
      curr = fwdList.erase_after(prev);
    }
  }
  cout << "Remove even and double odd in forward_list: ";
  for (auto a : fwdList)
    cout << a << " ";
  cout << endl;
}

void checkHowVectorGrows() {
  cout << "Checking how vector grows" << endl;

  vector<int> vi;
  cout << "\tsize: " << vi.size() << endl;
  cout << "\tcapacity: " << vi.capacity() << endl;

  for (int i=0; i<10; i++)
    vi.push_back(i);
  cout << "After pushing 10 elemns to it: " << endl;
  cout << "\tsize: " << vi.size() << endl;
  cout << "\tcapacity: " << vi.capacity() << endl;

//  cout << "Checking reserve(50)" << endl;
//  vi.reserve(50);
//  cout << "\tsize: " << vi.size() << endl;
//  cout << "\tcapacity: " << vi.capacity() << endl;

  cout << "Push back additional 7 elems" << endl;
  for (int i=0; i<7; i++)
    vi.push_back(i);
  cout << "\tsize: " << vi.size() << endl;
  cout << "\tcapacity: " << vi.capacity() << endl;

  cout << "Resize vi to 15" << endl;
  vi.resize(15);
  cout << "\tsize: " << vi.size() << endl;
  cout << "\tcapacity: " << vi.capacity() << endl;
//  cout << "vi.at(63): " << vi.at(63) << endl;
}

//#define CURRENT_SECTION_951
void initializeStringFromCharVector() {
  cout << "\ninitializeStringFromCharVector()" << endl;
  vector<char> vc = {'a', 'b', 'c'};
  vc.reserve(150);
  vector<char>::iterator it = vc.begin();

  string str(vc.begin(), vc.end());
  cout << "str: " << str << endl;

  for (int i=3; i<10; i++) {
    char tempc;
    cin.get(tempc);
    vc.push_back(tempc);
  }

  cout << "vc: ";
  for (auto c : vc) {
    cout << c << " ";
  }
  cout << endl;
}

#define SECTION_952
void useInsertAndEraseFunctionsWithIterators() {
  cout << "\nuseInsertAndEraseFunctionsWithIterators():\n";
  string s1 = "I am going there tho.";
  string s2 = "We can make it thru.";
  string s3 = "We need to think tho, that together we will make it thru. Have faith.";
  vector<string> vs = {s1, s2, s3};

  vector<string> oldValues = {"tho", "thru"};
  vector<string> newValues = {"though", "through"};

  for (auto s : vs) {
    for (int i=0; i<oldValues.size(); ++i) {
      size_t pos = s.find(oldValues.at(i));
      if (pos != string::npos) {
        string::iterator fromIt = s.begin() + pos;
        string::iterator toIt = fromIt + oldValues.at(i).length();
        fromIt = s.erase(fromIt, toIt);
        s.insert(fromIt, newValues.at(i).begin(), newValues.at(i).end());
      }
    }
    cout << s << endl;
  }
}

void useIndexAndReplaceFunction() {
  cout << "\nuseIndexAndReplaceFunction():" << endl;
  string s1 = "I am going there tho.";
  string s2 = "We can make it thru.";
  string s3 = "We need to think tho, that together we will make it thru. Have faith.";
  vector<string> vs = {s1, s2, s3};

  vector<string> oldValues = {"tho", "thru"};
  vector<string> newValues = {"though", "through"};

  for (string s : vs) {
    for (int i=0; i<oldValues.size(); ++i) {
      string::size_type pos = s.find(oldValues.at(i));
      if (pos != string::npos) {
        s.replace(pos, oldValues.at(i).length(), newValues.at(i));
      }
    }
    cout << s << endl;
  }
}

void addPrefixMrOrMsAndSuffixJrOrIIIUsingIteratorsInInsertFunction() {
  cout << "\naddPrefixMrOrMsAndSuffixJrOrIIIUsingIteratorsInInsertFunction():\n";

  vector<string> prefixes = {"Mr. ", "Ms. "};
  vector<string> suffixes = {" Jr.", " III"};

  string wiliam = "Wiliam Shakespeare";
  string margaret = "Margaret Thatcher";
  string son = "Wiliam Shakespeare";
  string grandson = "Wiliam Shakespeare";

  wiliam.insert(wiliam.begin(), prefixes.at(0).begin(), prefixes.at(0).end());
  cout << wiliam << endl;
  margaret.insert(margaret.begin(), prefixes.at(1).begin(), prefixes.at(1).end());
  cout << margaret << endl;
  son.insert(son.end(), suffixes.at(0).begin(), suffixes.at(0).end());
  cout << son << endl;
  grandson.insert(grandson.end(), suffixes.at(1).begin(), suffixes.at(1).end());
  cout << grandson << endl;
}

void addPrefixMrOrMsAndSuffixJrOrIIIUsingPositionAndLengthInInsertFunction() {
  cout << "\naddPrefixMrOrMsAndSuffixJrOrIIIUsingPositionsAndLengthsInInsertFunction():\n";

  vector<string> prefixes = {"Mr. ", "Ms. "};
  vector<string> suffixes = {" Jr.", " III"};

  string wiliam = "Wiliam Shakespeare";
  string margaret = "Margaret Thatcher";
  string son = "Wiliam Shakespeare";
  string grandson = "Wiliam Shakespeare";

  wiliam.insert(0, prefixes.at(0));
  cout << wiliam << endl;
  margaret.insert(0, prefixes.at(1));
  cout << margaret << endl;
  son.insert(son.length(), suffixes.at(0));
  cout << son << endl;
  grandson.insert(grandson.length(), suffixes.at(1));
  cout << grandson << endl;
}

#define SECTION_953
void findNumericAndAlphabeticCharInStringUsingFindFirstOf() {
  cout << "\findNumericAndAlphabeticCharInStringUsingFindFirstOf():\n";
  string s = "ab2c3d7R4E6";

  string numbers = "0123456789";
  string::size_type pos = 0;
  while ( (pos = s.find_first_of(numbers, pos)) != string::npos ) {
    cout << "Found number " << s.at(pos) << " at index " << pos << endl;
    ++pos;
  }

  string alphabet = "abcdefghijklmnopqrstUwxyzABCDEFGHIJKLMNOPQRSTUWXYZ";
  pos = 0;
  while ( (pos = s.find_first_of(alphabet, pos)) != string::npos) {
    cout << "Found alphabetic char " << s.at(pos) << " at index " << pos << endl;
    ++pos;
  }
}

void findNumericAndAlphabeticCharInStringUsingFindFirstNotOf() {
  cout << "\nfindNumericAndAlphabeticCharInStringUsingFindFirstNotOf():\n";
  string s = "ab2c3d7R4E6";

  string alphabet = "abcdefghijklmnopqrstUwxyzABCDEFGHIJKLMNOPQRSTUWXYZ";
  string::size_type pos = 0;
  while ( (pos = s.find_first_not_of(alphabet, pos)) != string::npos ) {
    cout << "Found number " << s.at(pos) << " at index " << pos << endl;
    ++pos;
  }

  string numbers = "0123456789";
  pos = 0;
  while ( (pos = s.find_first_not_of(numbers, pos)) != string::npos ) {
    cout << "Found alphabetic char " << s.at(pos) << " at index " << pos << endl;
    ++pos;
  }
}

int main()
{
  forward_list<string> fwdLst2 = {"stringA", "stringB", "stringC", "stringD" };
  insertStringToFwdLst(fwdLst2, "stringA", "stringCA");
  cout << "fwdLst2 after inserted string: ";
  for (auto str : fwdLst2)
    cout << str << " ";
  cout << endl;



  vector<int> vi;
  for (int i=0; i<10; ++i)
    vi.push_back(i);
  if ( isInVec(vi.begin(), vi.end(), 10) == vi.end() )
    cout << "not found" << endl;
  else
    cout << "is in vector" << endl;

  list<int> lst;
  list<int>::iterator iter1 = lst.begin(), iter2 = lst.end();
  while (iter1 != iter2) {
    cout << "less" << endl;
  }

  vector<int> vi1;
  vi.push_back(1);
  vector<int> vi2(10);
  vector<int> vi3(10,2);
  vector<int> vi4 = {1,2,3};
  vector<int> vi5 = vi4;
  vector<int> vi6(vi5);
  vector<int> vi0(vi2.begin(), vi2.end());

  list<string> authors = { "Milton", "Shakespeare", "Austen" };
  forward_list<string> words(authors.begin(), authors.end());

  list<int> l1(10,2);
  vector<double> vd(l1.begin(), l1.end());
  vector<double> vd2(vector<double>(vi3));

  list<const char*> lnames = {"John", "Mark", "Figo", "George"};
  vector<string> vnames;
  vnames.assign(lnames.begin(), lnames.end());
  for (auto name : vnames)
    cout << name << endl;

  vector<int> vec1 = {1,2,3,4,5,7,8,9};
  vector<int> vec2 = {1,2,3,4,5,6,7,8};
  vector<int> vec3 = {1,2,3,4};
  vector<int> vec4 = {1,2,3,4,5,7,8,9};
  cout << "comparing vecs: " << compareVecI(vec1, vec2) << endl;
  cout << "comparing vecs: " << compareVecI(vec1, vec3) << endl;
  cout << "comparing vecs: " << compareVecI(vec2, vec3) << endl;
  cout << "comparing vecs: " << compareVecI(vec1, vec4) << endl;

  list<int> li1 = {1,2,3,4,5,7,8, 9};
  cout << "comparing vec1 and li1: " << compareVecILI(vec1, li1) << endl;

//  list<string> ls;
//  string s;
//  int i = 0;
//  while (i<3 && cin >> s) {
//    ls.emplace_back(s);
//    ++i;
//  }
//
//  for (auto it=ls.begin(); it!=ls.end(); it++) {
//    cout << *it << endl;
//  }

  list<int> li2;
  for (int i=0; i<10; ++i) {
    li2.emplace_back(i);
  }

  deque<int> diEven, diOdd;
  for (auto num : li2) {
    if (num%2==0)
      diEven.emplace_back(num);
    else
      diOdd.emplace_back(num);
  }
  for (auto a : diEven)
    cout << a << " ";
  cout << endl;
  for (auto a : diOdd)
    cout << a << " ";
  cout << endl;


  vector<int> vi7;
  vi7.emplace_back(10);
  cout << "value at 0 using at: " << vi7.at(0) << endl;
  cout << "value at 0 using subscript oper: " << vi7[0] << endl;
  cout << "value at 0 using front: " << vi7.front() << endl;
  cout << "value at 0 using begin(): " << *(vi7.begin()) << endl;
  vi7.erase(vi7.begin(), vi7.begin());
  cout << "value at 0 using at: " << vi7.at(0) << endl;

  int ia[] = {0,1,1,2,3,5,8,13,21,55,89};
  vector<int> vi8( ia, ia + sizeof(ia)/sizeof(ia[0]) );
  cout << "vi8: ";
  for (auto a : vi8)
    cout << a << " ";
  cout << endl;
  list<int> li3( ia, ia + sizeof(ia)/sizeof(ia[0]) );
  cout << "li3: ";
  for (auto a : li3)
    cout << a << " ";
  cout << endl;

  vector<int>::iterator vit = vi8.begin();
  while(vit!= vi8.end()) {
    if (!*vit%2)
      vit = vi8.erase(vit);
    else
      vit++;
  }
  cout << "vi8 with evens removed: ";
  for (auto a : vi8)
    cout << a << " ";
  cout << endl;

  list<int>::iterator lit = li3.begin();
  while (lit != li3.end()) {
    if (*lit%2)
      lit = li3.erase(lit);
    else
      lit++;
  }
  cout << "li3 with odds removed: ";
  for (auto a : li3)
    cout << a << " ";
  cout << endl;

  forward_list<int> fli = {0,1,2,3,4,5,6,7,8,9};
  auto prev = fli.before_begin();
  auto curr = fli.begin();
  while (curr != fli.end()) {
    if (*curr%2)
      curr = fli.erase_after(prev);
    else {
      prev = curr;
      curr++;
    }
  }
  cout << "fw list after removing odds: ";
  for (auto a : fli)
    cout << a << " ";
  cout << endl;


  list<int> vi9 = {0,1,2,3,4,5,6,7,8,9};
  auto iter = vi9.begin();
  while (iter != vi9.end()) {
    if (*iter%2){
      iter = vi9.insert(iter, *iter);
      iter++;
      iter++;
    } else
      iter = vi9.erase(iter);
  }

  cout << "Remove even and double odds in list: ";
  for (auto a : vi9)
    cout << a << " ";
  cout << endl;

  removeEvenAndDoubleOddInFwdList();

  vector<int> vi10 = {0,1,2,3,4,5,6,7,8,9};
  auto iter10 = vi10.begin();
  while (iter10 != vi10.end()) {
    if (*iter10%2){
      iter10 = vi10.insert(iter10, *iter10++);
      iter10++;
    } else
      iter10 = vi10.erase(iter10);
  }
  cout << "Remove even and double odds in list #2: ";
  for (auto a : vi10)
    cout << a << " ";
  cout << endl;

  checkHowVectorGrows();

  #ifdef CURRENT_SECTION_951
  initializeStringFromCharVector();
  #endif // CURRENT_SECTION_951

  #ifdef SECTION_952
  useInsertAndEraseFunctionsWithIterators();
  useIndexAndReplaceFunction();
  addPrefixMrOrMsAndSuffixJrOrIIIUsingIteratorsInInsertFunction();
  addPrefixMrOrMsAndSuffixJrOrIIIUsingPositionAndLengthInInsertFunction();
  #endif // SECTION_952

  #ifdef SECTION_953
  findNumericAndAlphabeticCharInStringUsingFindFirstOf();
  findNumericAndAlphabeticCharInStringUsingFindFirstNotOf();
  #endif // SECTION_953

  return 0;
}
