#include <iostream>

using namespace std;

int nwd(int a, int b)
{
    int result=1, a_dividers[20], b_dividers[20];
    for (int i=0; i<20; i++)
    {
        a_dividers[i]=1;
        b_dividers[i]=1;
    }

    if (a>b)
    {
        int temp=a;
        a=b;
        b=temp;
    }

    // create dividers of a number and place it in a_dividers array
    int i=2;
    int j=1;
    while (a>1)
    {
        if (a%i>0)
            i++;
        else
        {
            a=a/i;
            a_dividers[j]=i;
            j++;
        }
    }

    // create dividers of a number and place it in b_dividers array
    i=2;
    int m=1;
    while (b>1)
    {
        if (b%i>0)
            i++;
        else
        {
            b=b/i;
            b_dividers[m]=i;
            m++;
        }
    }

    for (int x=0; x<20; x++) // x is a_dividers iterator
    {
        for (int y=0; y<20; y++) // y is b_dividers iterator
        {
            int found=false;
            if (a_dividers[x]==b_dividers[y])
            {
                b_dividers[y]=0;
                found=true;
                break;
            }
            if (found==false && y==19)
            {
                a_dividers[x]=0;
            }
        }
    }

    for (int i=0; i<20; i++)
    {
        if (a_dividers[i]!=0)
        {
            result*=a_dividers[i];
        }
    }

    return result;
}

int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        int a, b, result=1;//a_dividers[20], b_dividers[20];
        cin >> a >> b;
        result = nwd(a, b);
        cout << result << endl;
    }
    return 0;
}
