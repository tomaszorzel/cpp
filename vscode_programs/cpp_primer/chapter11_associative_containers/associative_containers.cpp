#include <iostream>
#include <fstream>
#include <map>
#include <iterator>
#include <algorithm>
#include <set>
#include <vector>

#define SECTION_11_1
void countWordsInInput() {
    std::cout << "countWordsInInput():\n";
    std::string fileName = "words.txt";
    std::ifstream inputFile(fileName);
    if (inputFile) {
        std::map<std::string, size_t> words_counts;
        std::set<std::string> exclude = {"the", "but", "and", "or", "an", "a", "of"};
        std::string word;
        while (inputFile >> word) {
            word = word.substr(0, word.find(','));
            word = word.substr(0, word.find('.'));
            std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c) {return std::tolower(c);} );
            if (exclude.find(word) == exclude.end())
                ++words_counts[word];
        }

        for (auto elem : words_counts) {
            std::cout << elem.first << " occurs " << elem.second << ((elem.second>1) ? " times" : " time") << std::endl; 
        }
    } else {
        std::cerr << "Failed to open " << fileName << std::endl;
    }
}

#define SECTION_11_2_1
void useMapToStoreFamilysLastNameAndChildrenInVec() {
    std::cout << "\nuseMapToStoreFamilysLastNameAndChildrenInVec():\n";
    std::map<std::string, std::vector<std::string>> families {
        {"Murphy", {"Tom", "Dina", "John"}},
        {"Wachowski", {"Sonic", "Elizabeth"}},
        {"Smith", {"Jojo","Anne","Will","Jack"}}
    };
    std::cout << "Map's content (families with children):\n";
    for (auto family : families) {
        std::cout << "Family's last name: " << family.first << ". Family's children: ";
        for (auto it=family.second.begin(); it!=family.second.end(); ++it) {
            std::cout << *it << ((it==family.second.end()-1) ? ".\n" :  ", ");
        }
    }

    // add new family to the map
    families.insert({"Geller", {"Monica", "Ross"}});
    std::cout << "Map's new content (families with children):\n";
    for (auto family : families) {
        std::cout << "Family's last name: " << family.first << ". Family's children: ";
        for (auto it=family.second.begin(); it!=family.second.end(); ++it) {
            std::cout << *it << ((it==family.second.end()-1) ? ".\n" :  ", ");
        }
    }

    // add new children to the existing family in the map
    families["Geller"].push_back("Chandler");
    std::cout << "Map's new content (families with children; Chandler added to Geller's family):\n";
    for (auto family : families) {
        std::cout << "Family's last name: " << family.first << ". Family's children: ";
        for (auto it=family.second.begin(); it!=family.second.end(); ++it) {
            std::cout << *it << ((it==family.second.end()-1) ? ".\n" :  ", ");
        }
    }
}

void useVecToStoreExcludedWords() {
    std::cout << "\nuseVecToStoreExcludedWords():\n";
    std::string fileName = "words.txt";
    std::ifstream inputFile(fileName);
    if(inputFile) {
        std::map<std::string, std::size_t> words_counts;
        std::vector<std::string> exclude = {"the", "but", "and", "or", "an", "a", "of"};
        std::string word;
        while (inputFile >> word) {
            word = word.substr(0, word.find(","));
            word = word.substr(0, word.find("."));
            std::transform( word.begin(), word.end(), word.begin(), [](unsigned char c) {return std::tolower(c);} );
            
            // check if current word should be skipped
            if (std::find(exclude.begin(), exclude.end(), word) == exclude.end())
                ++words_counts[word];
        }
        for (auto word : words_counts) {
            std::cout << word.first << " occurs " << word.second << ( (word.second > 1) ? " times\n" : " time\n" );
        }
    } else {
        std::cerr << "Failed to read " << fileName << "." << std::endl;
    }
}

int main() {
    #ifdef SECTION_11_1
    countWordsInInput();
    #endif // SECTION_11_1

    #ifdef SECTION_11_2_1
    useMapToStoreFamilysLastNameAndChildrenInVec();
    useVecToStoreExcludedWords();
    #endif // SECTION_11_2_1

    return 1;
}