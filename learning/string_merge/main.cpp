#include <iostream>
using namespace std;
#include <cstring>

#define T_SIZE 1001

char* string_merge(char *, char *);

int main()
{
    int t,n;
    char S1[T_SIZE], S2[T_SIZE], *S;
    cin >> t; /* wczytaj liczbe testow */
    while(t){
        cin >> S1 >> S2;
        S=string_merge(S1,S2);
        cout << S << endl;
        delete[] S;
        t--;
    }
    return 0;
}

char * string_merge(char * firstText, char * secondText)
{
    int lengthOfFirstText = strlen(firstText);
    int lengthOfSecondText = strlen(secondText);
    int lengthOfResult = (lengthOfFirstText < lengthOfSecondText) ? 2*lengthOfFirstText : 2*lengthOfSecondText;
    char * result = new char[lengthOfResult+1];
    int k=0,j=0;
    for (; k<lengthOfResult; j++)
    {
        result[k++] = firstText[j];
        result[k++] = secondText[j];
    }
    result[k] ='\0';
    return result;
}
