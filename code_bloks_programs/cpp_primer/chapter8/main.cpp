#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

//istream &read(istream &is) {
//  string in;
//  while (is >> in && !is.eof()) {
//    cout << in << endl;
//  }
//  is.clear();
//  return is;
//}

vector<string> &readFile(string inFileName, vector<string> &vs) {
  ifstream inFile(inFileName);
  if (inFile) {
    string s;
    while (inFile >> s) {
      vs.push_back(s);
    }
  } else {
    cout << "Error: file not opened" << endl;
  }
  return vs;
}

//int main(int argc, char *argv[])
int main()
{
  vector<string> vs;
  vs = readFile("famous_names.txt", vs);
  for (auto s : vs)
    cout << s << endl;
  return 0;
}
