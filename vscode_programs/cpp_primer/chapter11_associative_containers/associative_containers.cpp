#include <iostream>
#include <fstream>
#include <map>
#include <iterator>
#include <algorithm>
#include <set>
#include <vector>
#include <list>
#include <sstream>
#include <unordered_map>
#include <functional>

#include "book_store.h"

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

#define SECTION_11_2_2
void defineMapKeyStringValueList() {
    std::cout << "\ndefineMapKeyStringValueList():\n";
    std::map<std::string, std::list<int>> sliMap;
}

void initializeMapFromVecAndList() {
    std::cout << "\ninitializeMapFromVecAndList():\n";
    std::vector<int> vi = {1,2,3,4,5,6,5,6,7};
    std::map<int, int> mi;
    for (auto i : vi)
        mi.emplace(i, 0);
    
    std::cout << "Map initialized with vector: " << std::endl;
    for(auto pair : mi)
        std::cout << "key: " << pair.first << ", value: " << pair.second << ".\n";

    std::map<int, int> mi2;
    std::list<int> li = {1,2,3,4,5,6,5,6,7};
    for(auto i : li)
        mi2.emplace(i, 1);

    std::cout << "Map initialized with list: " << std::endl;
    for(auto pair : mi2)
        std::cout << "key: " << pair.first << ", value: " << pair.second << ".\n";
}

bool compareIsbn(const Sales_data &lhs, const Sales_data &rhs) {
    return lhs.isbn() < rhs.isbn();
}

void redefineBookstoreWOUsingDecltype() {
    std::cout << "\nredefineBookstoreWOUsingDecltype():\n";
    // std::multiset<Sales_data, decltype(compareIsbn)*> bookstore(compareIsbn);
    std::multiset<Sales_data, bool(*)(const Sales_data&, const Sales_data&)> bookstore(compareIsbn);

    // For section 11.3.1:
    std::multiset<Sales_data, bool(*)(const Sales_data&, const Sales_data&)>::iterator it = bookstore.begin();
}

#define SECTION_11_2_3
void readSeqOfStringsAndIntsAndStoreAsPairInVec() {
    std::cout << "\nreadSeqOfStringsAndIntsAndStoreAsPairInVec():\n";
    std::string fileName("seq_of_words_and_ints.txt");
    std::ifstream inputFile(fileName);
    if (inputFile){
        std::vector<std::pair<std::string, int>> vp;
        // std::pair<std::string, int> p;
        std::string word;
        int i;
        while (inputFile >> word) {
            inputFile >> i;
            // vp.push_back(make_pair(word, i));
            // vp.push_back( std::pair<std::string, int>(word, i) );
            std::pair<std::string, int> p = {word, i};
            vp.push_back(p);
        }
        std::cout << "Vector of pairs read from " << fileName << ": " << std::endl;
        for (auto p : vp) {
            std::cout << p.first << ", " << p.second << std::endl;
        }
    } else {
        std::cerr << "Failed to open " << fileName <<  std::endl;
    }
}

void useMapToStoreFamilysLastNameAndChildrenInVec2() {
    std::cout << "\nuseMapToStoreFamilysLastNameAndChildrenInVec():\n";
    std::map<std::string, std::vector<std::pair<std::string, std::string>>> families {
        {"Murphy", { {"Tom","09.09.2000"}, {"Dina", "05.01.1999"}, {"John", "03.03.1994"} }},
        {"Wachowski", { {"Sonic", "02.02.1992"}, {"Elizabeth", "01.01.2001"} }},
        {"Smith", { {"Jojo", "02.04.1997"}, {"Anne", "05.06.2005"}, {"Will", "02.05.1995"}, {"Jack", "06.06.1996"}}}
    };
    std::cout << "Map's content (families with children):\n";
    for (auto family : families) {
        std::cout << "Family's last name: " << family.first << ". Family's children: ";
        for (auto it=family.second.begin(); it!=family.second.end(); ++it) {
            std::cout << (*it).first << " (" << (*it).second << ")" << ((it==family.second.end()-1) ? ".\n" :  ", ");
        }
    }

    // add new family to the map
    families.insert({"Geller", {{"Monica", "03.05.1970"}, {"Ross", "25.11.1974"}}});
    std::cout << "Map's new content (families with children):\n";
    for (auto family : families) {
        std::cout << "Family's last name: " << family.first << ". Family's children: ";
        for (auto it=family.second.begin(); it!=family.second.end(); ++it) {
            std::cout << (*it).first << " (" << (*it).second << ")" << ((it==family.second.end()-1) ? ".\n" :  ", ");
        }
    }

    // add new children to the existing family in the map
    families["Geller"].push_back(std::make_pair("Chandler", "24.10.1974"));
    std::cout << "Map's new content (families with children; Chandler added to Geller's family):\n";
    for (auto family : families) {
        std::cout << "Family's last name: " << family.first << ". Family's children: ";
        for (auto it=family.second.begin(); it!=family.second.end(); ++it) {
            std::cout << (*it).first << " (" << (*it).second << ")" << ((it==family.second.end()-1) ? ".\n" :  ", ");
        }
    }
}

void assignValueToAnElemeInMap() {
    std::cout << "\nassignValueToAnElemeInMap():\n";
    std::map<std::string, int> msi = {
        {"word", 0}
    };
    auto it = msi.begin();
    std::cout << it->first << ", " << it->second << std::endl;
    it->second = 6;
    std::cout << it->first << ", " << it->second << std::endl;
}

#define SECTION_11_3_1
void initializeMultiSetFromVec() {
    std::cout << "\ninitializeMultiSetFromVec():\n";
    std::vector<int> vi = {1,2,3,4,5,6};
    std::multiset<int> si;
    copy(vi.begin(), vi.end(), std::inserter(si, si.end()));
    for (auto i : si)
        std::cout << i << " ";
    std::cout << std::endl;

    // std::multiset<int> si2;
    // copy(vi.begin(), vi.end(), std::back_inserter(si2)); // it won't compile. Set does not have push_back member!
    // for (auto i : si2)
    //     std::cout << i << " ";
    // std::cout << std::endl;

    std::multiset<int> si3 = {6,7,8,9,10,11};
    std::vector<int> vi3;
    copy(si3.begin(), si3.end(), std::inserter(vi3, vi3.end()));
    for (auto i : vi3)
        std::cout << i << " ";
    std::cout << std::endl;

    std::vector<int> vi4;
    copy(si3.begin(), si3.end(), std::back_inserter(vi4));
    for (auto i : vi4)
        std::cout << i << " ";
    std::cout << std::endl;
}

void writeIterTypeWOAutoOrDecltype() {
    std::cout << "\nwriteIterTypeWOAutoOrDecltype():\n";
    std::map<std::string, size_t> word_count;
    // auto map_it = word_count.cbegin();
    std::map<std::string, size_t>::const_iterator map_it = word_count.cbegin();
}

void redefineMultisetWithMethodForOrderingProvidedWOAutoOrDecltype() {
    std::cout << "\nredefineMultisetWithMethodForOrderingProvidedWOAutoOrDecltype():\n";
    // std::multiset<Sales_data, decltype(compareIsbn)*> bookstore(compareIsbn);
    std::multiset<Sales_data, bool(*)(const Sales_data&, const Sales_data&)> bookstore(compareIsbn);

    // std::multiset<Sales_data, decltype(compareIsbn)*>::iterator it = bookstore.begin();
    std::multiset<Sales_data, bool(*)(const Sales_data&, const Sales_data&)>::iterator it = bookstore.begin();
}

#define SECTION_11_3_2
void countWordsInInputUseInsert() {
    std::cout << "\ncountWordsInInputUseInsert():\n";
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
            if (exclude.find(word) == exclude.end()) {
                // ++words_counts[word];
                std::pair<std::map<std::string, size_t>::iterator, bool> ret = words_counts.insert(std::pair<std::string, size_t>(word, 1));
                // auto ret = words_counts.insert(std::make_pair(word, 1));
                if (!ret.second) {
                    ++(ret.first->second);
                }
            }
        }

        for (auto elem : words_counts) {
            std::cout << elem.first << " occurs " << elem.second << ((elem.second>1) ? " times" : " time") << std::endl; 
        }
    } else {
        std::cerr << "Failed to open " << fileName << std::endl;
    }
}

void practiceInsertForMap() {
    std::cout << "\npracticeInsertForMap():\n";
    std::map<std::string, std::vector<int>> map;
    std::pair<std::map<std::string, std::vector<int>>::iterator, bool> res = map.insert(std::make_pair("John", std::vector<int>{1,2,3,4,5}));
    if (res.second) {
        std::cout << "Inserting to map is successful. Map content is: ";
        std::cout << res.first->first << ", ";
        for (int i : res.first->second) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }
}

void useMultimapToStoreFamilysLastNameAndChildrenInVec() {
    std::cout << "\nuseMultimapToStoreFamilysLastNameAndChildrenInVec():\n";
    std::multimap<std::string, std::vector<std::string>> families {
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
    // families["Geller"].push_back("Chandler");
    auto it = families.find("Geller");
    if (it != families.end()) {
        it->second.push_back("Chandler");
        std::cout << "Map's new content (families with children; Chandler added to Geller's family):\n";
        for (auto family : families) {
            std::cout << "Family's last name: " << family.first << ". Family's children: ";
            for (auto it=family.second.begin(); it!=family.second.end(); ++it) {
                std::cout << *it << ((it==family.second.end()-1) ? ".\n" :  ", ");
            }
        }
    }
}

#define SECTION_11_3_5
void practiceFindOnMap() {
    std::cout << "\npracticeFindOnMap():\n";
    std::map<std::string, std::vector<int>> mapStrToIntsVec = {
        {"Joey", {1,2,3,4,5}},
        {"Rachel", {6,7,8,9}},
        {"Ross", {10,11,12,13}},
        {"Chandler", {20,21,22,23}}
    };
    std::map<std::string, std::vector<int>>::iterator it = mapStrToIntsVec.find("Ross");
    if (it != mapStrToIntsVec.end())
        std::cout << "\"Ross\" found in a map.\n";
}

void storeAthorsAndTheirWorksInMultimapAndPracticeErase() {
    std::cout << "\nstoreAthorsAndTheirWorksInMultimapAndPracticeErase():\n";
    std::multimap<std::string, std::string> authorsAndBooks {
        {"Mateusz Grzesiak", "Success and change"}, 
        {"Daniel Goleman", "Inteligencja emocjonalna"},
        {"Mateusz Grzesiak", "Ty myśl"},
        {"John Mcdougal", "Zdrowie bez recepty czyli skrobia, która leczy"},
        {"Daniel Goleman", "Trwała przemiana"}
    };

    std::cout << "Multimap containing authors and books:" << std::endl;
    for (auto pair : authorsAndBooks) {
        std::cout << "Author: " << pair.first << ", book: " << pair.second << std::endl;
    }

    std::map<std::string, std::string>::iterator pos = authorsAndBooks.find("Mateusz Grzesiak");
    if (pos != authorsAndBooks.end()) {
        authorsAndBooks.erase(pos);
        std::cout << "Multimap containing authors and books after calling erase on Mateusz Grzesiak author:" << std::endl;
        for (auto pair : authorsAndBooks) {
            std::cout << "Author: " << pair.first << ", book: " << pair.second << std::endl;
        }
    }

    pos = authorsAndBooks.find("Tadeusz Niwinski");
    if (pos != authorsAndBooks.end()) {
        authorsAndBooks.erase(pos);
        std::cout << "Multimap containing authors and books after calling erase on Tadeusz Niwinski author:" << std::endl;
        for (auto pair : authorsAndBooks) {
            std::cout << "Author: " << pair.first << ", book: " << pair.second << std::endl;
        }
    } else {
        std::cout << "Tadeusz Niwinski not found on a map!" << std::endl;
    }
}

void printAuthorsAndBooksStoredInMultimap() {
    std::cout << "\nprintAuthorsAndBooksStoredInMultimap():\n";
    std::multimap<std::string, std::string> authorsAndBooks {
        {"Mateusz Grzesiak", "Success and change"}, 
        {"Daniel Goleman", "Inteligencja emocjonalna"},
        {"Mateusz Grzesiak", "Ty myśl"},
        {"John Mcdougal", "Zdrowie bez recepty czyli skrobia, która leczy"},
        {"Daniel Goleman", "Trwała przemiana"}
    };
    
    std::cout << "Authors and books stored in multimap:\n";
    for (auto author : authorsAndBooks) {
        std::cout << author.first << " \"" << author.second << "\"\n";
    }

    std::cout << "Authors and theirs books:\n";
    for (auto it = authorsAndBooks.begin(); it != authorsAndBooks.end(); ) {
        std::cout << it->first << ": ";
        auto itPair = authorsAndBooks.equal_range(it->first);
        std::multimap<std::string, std::string>::iterator beg, end;
        for (beg = itPair.first, end = itPair.second; beg != end; ++beg) {
            std::cout << "\"" << beg->second << "\" ";
        }
        std::cout << std::endl;
        it = end;
    }
}

#define SECTION_11_3_6
const std::string & transform(const std::string &word, const std::map<std::string, std::string> &transformationMap) {
    auto it = transformationMap.find(word);
    if(it != transformationMap.cend()) {
        return it->second;
    } else {
        return word;
    }
}

int transformText() {
    std::cout << "\ntransformText():\n";
    std::string wordTransformationFileName = "word_transformation.txt";
    std::ifstream wordTransformationFile(wordTransformationFileName);
    if (!wordTransformationFile) {
        std::cout << "Failed to read " << wordTransformationFileName << "!" << std::endl;
        return 1;
    }
    std::map<std::string, std::string> wordTransformationMap;
    std::string shortcut, line;
    while (getline(wordTransformationFile, line)) {
        std::stringstream ss(line);
        ss >> shortcut;
        getline(ss, line);
        line.erase(line.begin()); // erase first " " sign in meaning of the shorcut
        if (!line.empty())
            wordTransformationMap[shortcut] = line;
        else
            std::cerr << "No value for \"" << shortcut << "\"!\n";
    }
    wordTransformationFile.close();

    if (wordTransformationMap.empty()) {
        std::cout << "Map is empty, something went wrong!" << std::endl;
        return 1;
    } else {
        std::cout << "Map's content: \n";
        for (auto it = wordTransformationMap.begin(); it != wordTransformationMap.end(); ++it) {
            std::cout << it->first << " " << it->second << std::endl;
        }
    }

    std::string textToTransformFileName = "text_to_transform.txt";
    std::ifstream textToTransformFile(textToTransformFileName);
    if (!textToTransformFile) {
        std::cout << "Failed to read " << textToTransformFileName << "!" << std::endl;
    }

    std::cout << "Text tranformed:\n";
    std::string text;
    std::set<std::string> exclude = {"?", ",", "."};
    while (getline(textToTransformFile, text)) {
        std::stringstream sstream(text);
        std::string word;
        bool firstWord = true;
        while (sstream >> word) {
            if (firstWord)
                firstWord = false;
            else 
                std::cout << " ";
            
            bool excludedSignFound = false;
            std::string sign;
            for (auto s : exclude) {
                auto pos = word.find(s);
                if (pos != std::string::npos) {
                    excludedSignFound = true;
                    word.erase(pos);
                    sign = s;
                    break; // assume that there might be only one sign in a word 
                }
            }
            std::cout << transform(word, wordTransformationMap);
            if (excludedSignFound) {
                std::cout << sign;
            }
        }
        std::cout << std::endl;
    }
    return 0;
}

#define SECTION_11_4
void countWordsInInputUsingUnorderedMap() {
    std::cout << "\ncountWordsInInputUsingUnorderedMap():\n";
    std::string fileName = "words.txt";
    std::ifstream inputFile(fileName);
    if (inputFile) {
        std::unordered_map<std::string, size_t> words_counts;
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

const std::string & transform(const std::string &word, const std::unordered_map<std::string, std::string> &transformationMap) {
    auto it = transformationMap.find(word);
    if(it != transformationMap.cend()) {
        return it->second;
    } else {
        return word;
    }
}

int transformTextUsingUnorderedMap() {
    std::cout << "\ntransformTextUsingUnorderedMap():\n";
    std::string wordTransformationFileName = "word_transformation.txt";
    std::ifstream wordTransformationFile(wordTransformationFileName);
    if (!wordTransformationFile) {
        std::cout << "Failed to read " << wordTransformationFileName << "!" << std::endl;
        return 1;
    }
    std::unordered_map<std::string, std::string> wordTransformationMap;
    std::string shortcut, line;
    while (getline(wordTransformationFile, line)) {
        std::stringstream ss(line);
        ss >> shortcut;
        getline(ss, line);
        line.erase(line.begin()); // erase first " " sign in meaning of the shorcut
        if (!line.empty())
            wordTransformationMap[shortcut] = line;
        else
            std::cerr << "No value for \"" << shortcut << "\"!\n";
    }
    wordTransformationFile.close();

    if (wordTransformationMap.empty()) {
        std::cout << "Map is empty, something went wrong!" << std::endl;
        return 1;
    } else {
        std::cout << "Map's content: \n";
        for (auto it = wordTransformationMap.begin(); it != wordTransformationMap.end(); ++it) {
            std::cout << it->first << " " << it->second << std::endl;
        }
    }

    std::string textToTransformFileName = "text_to_transform.txt";
    std::ifstream textToTransformFile(textToTransformFileName);
    if (!textToTransformFile) {
        std::cout << "Failed to read " << textToTransformFileName << "!" << std::endl;
    }

    std::cout << "Text tranformed:\n";
    std::string text;
    std::set<std::string> exclude = {"?", ",", "."};
    while (getline(textToTransformFile, text)) {
        std::stringstream sstream(text);
        std::string word;
        bool firstWord = true;
        while (sstream >> word) {
            if (firstWord)
                firstWord = false;
            else 
                std::cout << " ";
            
            bool excludedSignFound = false;
            std::string sign;
            for (auto s : exclude) {
                auto pos = word.find(s);
                if (pos != std::string::npos) {
                    excludedSignFound = true;
                    word.erase(pos);
                    sign = s;
                    break; // assume that there might be only one sign in a word 
                }
            }
            std::cout << transform(word, wordTransformationMap);
            if (excludedSignFound) {
                std::cout << sign;
            }
        }
        std::cout << std::endl;
    }
    return 0;
}

std::string returnSimpleString() {
    return "simpleString";
}

int main() {
    #ifdef SECTION_11_1
    countWordsInInput();
    #endif // SECTION_11_1

    #ifdef SECTION_11_2_1
    useMapToStoreFamilysLastNameAndChildrenInVec();
    useVecToStoreExcludedWords();
    #endif // SECTION_11_2_1

    #ifdef SECTION_11_2_2
    defineMapKeyStringValueList();
    initializeMapFromVecAndList();
    #endif // SECTION_11_2_2

    #ifdef SECTION_11_2_3
    readSeqOfStringsAndIntsAndStoreAsPairInVec();
    useMapToStoreFamilysLastNameAndChildrenInVec2();
    assignValueToAnElemeInMap();
    #endif // SECTION_11_2_3

    #ifdef SECTION_11_3_1
    initializeMultiSetFromVec();
    writeIterTypeWOAutoOrDecltype();
    redefineMultisetWithMethodForOrderingProvidedWOAutoOrDecltype();
    #endif // SECTION_11_3_1

    #ifdef SECTION_11_3_2
    countWordsInInputUseInsert();
    practiceInsertForMap();
    useMultimapToStoreFamilysLastNameAndChildrenInVec();
    #endif // SECTION_11_3_2

    #ifdef SECTION_11_3_5
    practiceFindOnMap();
    storeAthorsAndTheirWorksInMultimapAndPracticeErase();
    printAuthorsAndBooksStoredInMultimap();
    #endif // SECTION_11_3_5

    #ifdef SECTION_11_3_6
    transformText();
    #endif // SECTION_11_3_6

    #ifdef SECTION_11_4
    countWordsInInputUsingUnorderedMap();
    transformTextUsingUnorderedMap();
    std::cout << "Hash for simple string: " << std::hash<std::string>()(returnSimpleString()) << std::endl;
    #endif // SECTION_11_4

    return 1;
}