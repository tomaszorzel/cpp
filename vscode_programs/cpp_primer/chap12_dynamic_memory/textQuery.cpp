#include <memory>
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class QueryResult;
class TextQuery {
public:
    TextQuery(std::ifstream&);
    QueryResult query(const std::string&) const;
    using lineNo = std::vector<std::string>::size_type;
private:
    std::shared_ptr<std::vector<std::string>> file;
    std::map<std::string, std::shared_ptr<std::set<lineNo>>> wm;
};

TextQuery::TextQuery(std::ifstream &inFile) : file(new std::vector<std::string>){
    std::string text;
    while (getline(inFile, text)) {
        file->push_back(text);
        int n = file->size()-1; // current line
        std::istringstream line(text);
        std::string word;
        while (line >> word) {
            auto &lines = wm[word];
            if (!lines) {
                lines.reset(new std::set<lineNo>);
            }
            lines->insert(n);
        }
    }
}

class QueryResult {
    friend std::ostream &print(std::ostream&, const QueryResult&);
    using lineNo = std::vector<std::string>::size_type;
public:
    QueryResult(std::string s,
                std::shared_ptr<std::set<lineNo>> p,
                std::shared_ptr<std::vector<std::string>> f) :
                sought(s), lines(p), file(f) {}
private:
    std::string sought;
    std::shared_ptr<std::set<lineNo>> lines;
    std::shared_ptr<std::vector<std::string>> file;
};

QueryResult TextQuery::query(const std::string &sought) const {
    static std::shared_ptr<std::set<lineNo>> nodata(new std::set<lineNo>);
    auto loc = wm.find(sought);
    if (loc == wm.end()) {
        return QueryResult(sought, nodata, file);
    } else {
        return QueryResult(sought, loc->second, file);
    }
}

std::string make_plural(unsigned cnt, std::string word, std::string postfix) {
    return (cnt == 1) ? word : word+postfix;
}

std::ostream& print(std::ostream& os, const QueryResult &qr) {
    os << qr.sought << " occurs " << qr.lines->size() << " " << make_plural(qr.lines->size(), "time", "s") << std::endl;
    for (auto num : *qr.lines) {
        os << "\t(line " << num + 1 << ") " << *(qr.file->begin() + num) << std::endl;
    }
    return os;
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
    if (inFile.is_open()) {
        runQueries(inFile);
    } else {
        std::cout << "Failed to open " << inFileName << std::endl;
    }
    inFile.close();
}

int main() {
    findOccurencesInInputFile();
    return 0;
}