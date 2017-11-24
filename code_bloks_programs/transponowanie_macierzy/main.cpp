#include <iostream>
using namespace std;

int main()
{
    int m, n;
    cin >> m >> n;
    if (m > 200 || n > 200) return 0;

    int ** matrix = new int*[m];
    for (int i = 0; i < m; i++)
    {
        matrix[i] = new int[n];
        for (int j =0; j < n; j++)
        {
            cin >> matrix[i][j];
        }
    }

    for (int i=0; i<n; i++)
    {
        for (int j=0; j<m; j++)
        {
            cout << matrix[j][i];
            if (j != (m-1)) cout << " ";
        }
        cout << endl;
    }

    for (int i = 0; i < m; i++)
    {
        delete [] matrix[i];
    }
    delete [] matrix;

    return 0;
}
