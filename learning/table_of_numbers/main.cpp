#include <iostream>

using namespace std;

int main()
{
    int t,l,k;

    // let user type number of tests
    cin >> t;

    while (t)
    {
        cin >> l >> k;

        if (l<3 || l>100 || k<3 || k>100) return 0;
        int ** matrix = new int*[l];
        for (int i=0; i<l; i++)
        {
            matrix[i] = new int[k];
            for (int j=0; j<k; j++)
            {
                cin >> matrix[i][j];
            }
        }

        int temp = matrix[0][0];

        for (int i=0; i<(k-1); i++)
        {
            matrix[0][i] = matrix[0][i+1];
        }
        for (int i=0; i<(l-1); i++)
        {
            matrix[i][k-1] = matrix[i+1][k-1];
        }
        for (int i=(k-1); i>0; i--)
        {
            matrix[l-1][i] = matrix[l-1][i-1];
        }
        for (int i=(l-1); i>1; i--)
        {
            matrix[i][0] = matrix[i-1][0];
        }
        matrix[1][0] = temp;

        // print matrix
        for (int i=0; i<l; i++)
        {
            for (int j=0; j<k; j++)
            {
                cout << matrix[i][j];
                if (j != (k-1)) cout << " ";
            }
            cout << endl;
        }

        // make free allocated memory
        for (int i=0; i<l; i++)
        {
            delete [] matrix[i];
        }
        delete [] matrix;

        t--;
    }
    return 0;
}
