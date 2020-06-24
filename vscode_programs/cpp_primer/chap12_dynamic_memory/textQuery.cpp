#include <memory>
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

class TextQuery;
class QueryResult {
    std::shared_ptr<TextQuery> sptq;
public:
};

std::ostream& print(std::ostream& out, QueryResult &qr) {

    return out;
}

class TextQuery {
public:
    TextQuery(std::ifstream&);
    QueryResult query(const std::string&) const;
    using lineNo = std::vector<std::string>::size_type;
private:
    std::shared_ptr<std::vector<std::string>> file;
    std::map<std::string, std::shared_ptr<std::set<lineNo>>> wm;
};

TextQuery::TextQuery(std::ifstream &inFile) {
    std::string line;
    while (getline(inFile, line)) {
        linesInFile.push_back(line);
    }
}

void runQueries(std::ifstream &inFile) {
    TextQuery tq(inFile);
    while (true) {
        std::cout << "enter word to look for, or q to quit: ";
        std::string s;
        if ( !(std::cin >> s) || s == "q") break;
        print(std::cout, tq.query(s)) << std::endl;
    }
}

void findOccurencesInInputFile() {
    std::cout << "\nfindOccurencesInInputFile():\n";
    std::string inFileName("fileToFindOccurencesIn.txt");
    std::ifstream inFile(inFileName);
    if (!inFile) {
        runQueries(inFile);
    } else {
        std::cout << "Failed to open " << inFileName << std::endl;
    }
}

int main() {
    // findOccurencesInInputFile();
    return 0;
}