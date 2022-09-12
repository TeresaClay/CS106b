/*
 * File: main.cpp
 * --------------
 * Blank C++ project configured to use Stanford cslib and Qt
 */

#include "console.h"
#include "simpio.h"
#include "strlib.h"
using namespace std;

// version 1: for
/*
void starPatterns(int n)
{
    for (int i=0; i<n; i++){

        for (int j=0; j < n-1-i ; j++){
            cout << " " ;
        }

        for (int k=0; k < i+1; k++){
            cout << "* " ;
        }

     cout << endl;

    }
}
*/

// version 2 ( course )
void starPatterns (int n){
    for (int i=n-1; i >= 0; i--){

        for (int j=1-n; j<=n-1;j++){

            int sum=abs(i)+abs(j);

            if (sum < n  &&  sum % 2 == (n-1) % 2){
                cout << "*";
            } else {
                cout << " ";
            }
         }
        cout << endl;
     }

}


int main()
{
    int n = getInteger("number of layers ?");

    if (n==0){
        n = 6;
    }

    starPatterns(n);

    return 0;
}
