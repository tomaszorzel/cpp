#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <fstream>
#include <iterator>
// #include "Sales_item.h"
#include "book_store.h"

int main() {
    std::vector<Sales_data> vsd;
    Sales_data total;

    std::ifstream input("records.txt");
    if (input) {
        std::cout << "Input read successfully." << std::endl;
        if (read(input, total)) {
            vsd.push_back(total);
            Sales_data trans;
            while(read(input, trans)) {
                vsd.push_back(trans);
                if (total.isbn() == trans.isbn()) {
                    total.combine(trans);
                } else {
                    vsd.push_back(total);
                    total = trans;
                }
            }
        }
        std::ostream_iterator<Sales_data> out(std::cout, "");
        // copy(vsd.begin(), vsd.end(), out);
        // continue here, vsd content is weird
    } else {
        std::cerr << "Failed to read input file" << std::endl;
        return -1;
    }
    std::cout << "Program done." << std::endl;
    return 0;
}