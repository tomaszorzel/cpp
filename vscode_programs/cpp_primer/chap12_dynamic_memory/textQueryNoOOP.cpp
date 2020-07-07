#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <fstream>

int main() {
    // build the map
    std::map<std::string, std::set<std::vector<std::string>::size_type>> wm;
    std::vector<std::string> lines;

    std::string fileName("fileToFindOccurencesIn.txt");
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cout << "Failed to open file " << fileName << std::endl;
        return 1;
    }

    std::string line;
    while (getline(file, line)) {
        lines.push_back(line);
        auto n = lines.size() - 1;
        std::string word;
        std::stringstream ss(line);
        while (ss >> word) {
            auto &lineNumbers = wm[word];
            lineNumbers.insert(n);
        }
    }
    std::cout << lines.size() << std::endl;

    while (true) {
        std::cout << "enter word to look for, or q to quit: ";
        std::string sought;
        if ( !(std::cin >> sought) || sought=="q")
            break;

        if (wm.find(sought) != wm.end()) {
            std::cout << sought << " occurs " << wm.at(sought).size() << std::endl;
            for (auto& elem : wm.at(sought)) {
                std::cout << "\t(" << elem+1 << ") " << lines.at(elem) << std::endl;
            }
        }
        
    }

    file.close();
    return 0;
}