#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>

class QueryResult;
class TextQuery {
public:
    TextQuery(std::ifstream&);
    QueryResult query(const std::string&) const;
    using lineNo = std::vector<std::string>::size_type;
private:
    std::map<std::string, std::shared_ptr<std::set<lineNo>>> wordToLinesMap;
    std::shared_ptr<std::vector<std::string>> shpLinesFromFile;
};

TextQuery::TextQuery(std::ifstream &file) : shpLinesFromFile(new std::vector<std::string>) /*initialize pointer to vec to avoid */ {
    // build the map
    std::string line;
    while (getline(file, line)) {
        std::cout << line << std::endl;
        shpLinesFromFile->push_back(line); // segmentation fault was here :)
        auto n = shpLinesFromFile->size()-1; // current line
        std::string word;
        std::stringstream ss(line);
        while (ss >> word) {
            auto& shpSetOfLines = wordToLinesMap[word];
            if (!shpSetOfLines) {
                shpSetOfLines = std::make_shared<std::set<lineNo>>();
                // optionally: shpSetOfLines.reset(new std::set<lineNo>);
            }
            shpSetOfLines->insert(n);
        }
    }
}

class QueryResult {
    friend std::ostream& print(std::ostream&, const QueryResult&);
public:
    QueryResult(std::string sought, 
                std::shared_ptr<std::set<TextQuery::lineNo>> shpSetOfLines,
                std::shared_ptr<std::vector<std::string>> linesInFile) : 
                m_sought(sought),
                m_shpSetOfLines(shpSetOfLines),
                m_linesInFile(linesInFile)
                {}
private:
    std::string m_sought;
    std::shared_ptr<std::set<TextQuery::lineNo>> m_shpSetOfLines;
    std::shared_ptr<std::vector<std::string>> m_linesInFile;
};

QueryResult TextQuery::query(const std::string& s) const {
    static const std::shared_ptr<std::set<lineNo>> noData(new std::set<lineNo>);
    auto it = wordToLinesMap.find(s);
    if (it != wordToLinesMap.end())
        return QueryResult(s, it->second, shpLinesFromFile);
    else 
        return QueryResult(s, noData, shpLinesFromFile);
}

std::ostream& print(std::ostream &os, const QueryResult &qr) {
    os << qr.m_sought << " occurs " << qr.m_shpSetOfLines->size() << " times" << std::endl;
    for (auto no : *(qr.m_shpSetOfLines)) {
        os << "\t(" << no+1 << ") " << qr.m_linesInFile->at(no) << std::endl;
    }
    return os;
}

void runQueries() {
    std::cout << "enter word to look for or press q to quit: ";
    std::string sought;
    while ( (std::cin >> sought) && sought != "q") {
        std::string fileName("fileToFindOccurencesIn.txt");
        std::ifstream file(fileName);
        if (!file.is_open()) {
            std::cout << "Failed to open a file";
            return;
        } else {
            std::cout << "before creating tq" << std::endl;
            TextQuery tq(file);
            std::cout << "after creating tq" << std::endl;
            print(std::cout, tq.query(sought)) << std::endl;
        }
        std::cout << "enter word to look for or press q to quit: ";
    }
}

int main() {
    runQueries();
    return 0;
}