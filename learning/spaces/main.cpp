#include <iostream>
#include <cctype>
using namespace std;

int main()
{
    string sms;

    while (getline(cin, sms)) {
        int i=0;
        while (i<sms.length()) {
            bool nextLetterShouldBeGreat=false;
            while (sms[i]==' ' || sms[i]=='\n' || sms[i]=='\t') {
                if (sms[i]=='\n')
                    cout << endl;
                nextLetterShouldBeGreat=true;
                i++;
            }
            if (nextLetterShouldBeGreat)
                sms[i]=toupper(sms[i]);
            cout << sms[i];
            i++;
        }
        cout << endl;
    }

    return 0;
}
