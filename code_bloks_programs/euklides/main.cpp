#include <iostream>
using namespace std;

int main()
{
	int a, b, t;
	cin >> t;
	while(t)
	{
		cin >> a >> b;
        while (a!=b)
        {
            while (a>b)
            {
                int c=a/b;
                double d=a%b;
                if (d>0) a-=(b*c);
                else a-=(b*(c-1));
            }
            while (b>a)
            {
                int c=b/a;
                double d=b%a;
                if (d>0) b-=(a*c);
                else b-=(a*(c-1));
            }
        }
		cout << a+b << endl;
		t--;
	}
	return 0;
}
