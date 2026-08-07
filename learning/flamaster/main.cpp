#include <iostream>
#include <sstream>
#include <string>
using namespace std;

string to_string(int i)
{
    stringstream ss;
    ss << i;
    return ss.str();
}

int main()
{
    string testWord;
    string result;
    int c;
    cin >> c;
    while (c)
    {
        cin >> testWord;
        int counter=1;
        result=testWord[0];
        for (int i=1; i<testWord.length(); i++)
        {
            char temp=testWord[i-1];
            if (temp==testWord[i])
                counter++;
            if (temp!=testWord[i])
            {
                if (counter==2)
                {
                    result+=temp;
                }
                else if (counter>2)
                {
                    result+=to_string(counter);
                }
                result+=testWord[i];
                counter=1;
            }

            if (i==(testWord.length()-1) && counter==2)
                result+=testWord[i];
            else if (i==(testWord.length()-1) && counter!=1)
                result+=to_string(counter);
        }
        cout << result << endl;
        c--;
    }
    return 0;
}
