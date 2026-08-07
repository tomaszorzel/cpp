#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
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
    while (getline(inFile, s)) {
      istringstream iss(s);
      vs.push_back(iss.str());
    }
  } else {
    cout << "Error: file not opened" << endl;
  }
  return vs;
}

struct PersonInfo {
  string name;
  vector<string> phones;
};

//int main(int argc, char *argv[])
int main()
{
//  vector<string> vs;
//  vs = readFile("famous_names.txt", vs);
//  for (auto s : vs) {
//    istringstream iss(s);
//    string firstName, secondName, job;
//    iss >> firstName;
//    iss >> secondName;
//    iss >> job;
//    cout << firstName << " " << secondName << ", " << job << endl;
//  }

  string line, word;
  vector<PersonInfo> people;
  fstream peoplePhones("people_phones.txt");
  while (getline(peoplePhones, line)) {
    istringstream iss(line);
    PersonInfo person;
    iss >> person.name;
    while (iss >> word) {
      person.phones.push_back(word);
    }
    people.push_back(person);
  }
  for (auto person : people) {
    cout << person.name << " ";
    for (auto phone : person.phones)
      cout << phone << " ";
    cout << endl;
  }
  return 0;
}
