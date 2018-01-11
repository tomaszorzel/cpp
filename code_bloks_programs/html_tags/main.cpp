#include <iostream>

using namespace std;

int main()
{
    string line;
    while (getline(cin, line)) {
        int i=0;
        bool needsToMakeUppercase=false;
        while (i!=line.length()) {
            if (line[i]=='<') {
                needsToMakeUppercase=true;
            } else if (line[i]=='>') {
                needsToMakeUppercase=false;
            }

            if (needsToMakeUppercase) {
                line[i]=toupper(line[i]);
            }
            cout << line[i];
            i++;
        }
        cout << endl;
    }
    return 0;
}
