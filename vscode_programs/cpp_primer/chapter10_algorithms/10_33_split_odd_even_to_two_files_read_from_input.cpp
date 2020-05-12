#include <fstream>
#include <iostream>
#include <iterator>

int main(int argc, char *argv[])
{
    std::ifstream inFile(argv[1]);
    std::istream_iterator<int> in(inFile);
    std::istream_iterator<int> eof;

    std::ofstream outOddFile(argv[2]);
    std::ofstream outEvenFile(argv[3]);
    std::ostream_iterator<int> outOdd(outOddFile, " ");
    std::ostream_iterator<int> outEven(outEvenFile, "\n");

    while (in != eof) {
        if (*in % 2) {
            outOdd = *in++;
        } else {
            outEven = *in++;
        }
    }
    return 0;
}