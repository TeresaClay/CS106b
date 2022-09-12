/*
 * File: main.cpp
 * --------------
 * Blank C++ project configured to use Stanford cslib and Qt
 */

#include "console.h"
#include "simpio.h"
using namespace std;

/*
 *       1
 *      2 2
 *     3   3
 *    4     4
 *   5       5
 *  6 6 6 6 6 6
 *
*/



void starPatterns (int n){
    for (int i=n-1; i > 0; i--){

        for (int j=1-n; j<=n-1;j++){

            int sum=abs(i)+abs(j);

            if (sum < n  &&  abs(j)==(n-1-i)){
                cout << n-i;
            } else {
                cout << " ";
            }
         }
        cout << endl;
     }

    for (int j=0;j<n;j++){
        cout << n << " ";
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
