#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <fstream>
#include <iterator>
#include "Sales_item.h"

bool compareIsbn2(const Sales_item &sd1, const Sales_item &sd2) {
    return sd1.isbn() < sd2.isbn();
}

int main() {
    std::vector<Sales_item> vsd;
    Sales_item total;

    std::ostream_iterator<Sales_item> out(std::cout, "\n");

    std::ifstream input("records.txt");
    if (input) {
        if (read(input, total)) {
            Sales_item trans;
            while(read(input, trans)) {
                if (total.isbn() == trans.isbn()) {
                    total+=trans;
                } else {
                    vsd.push_back(total);
                    total = trans;
                }
            }
            vsd.push_back(total);
        }
        copy(vsd.begin(), vsd.end(), out);
    } else {
        std::cerr << "Failed to read input file" << std::endl;
        return -1;
    }

    std::cout << "Sorted vector: " << std::endl;
    sort(vsd.begin(), vsd.end(), compareIsbn2);
    copy(vsd.begin(), vsd.end(), out);

    std::cout << "New vec with accumulated transactions:" << std::endl;
    std::vector<Sales_item> vsd2;
    for (auto b = vsd.begin(), e = b; b != vsd.end(); b=e) {
        e = std::find_if_not(b, vsd.end(), [b](const Sales_item &sd) { return sd.isbn() == b->isbn(); });
        vsd2.push_back(std::accumulate(b, e, Sales_item(b->isbn())));
    }
    copy(vsd2.begin(), vsd2.end(), out); // accumulation does not work (https://github.com/jaege/Cpp-Primer-5th-Exercises/blob/master/ch10/Sales_item.h)

    std::cout << "Program done." << std::endl;
    return 0;
}