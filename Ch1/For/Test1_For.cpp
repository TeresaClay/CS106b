/*
 * File: main.cpp
 * --------------
 * Blank C++ project configured to use Stanford cslib and Qt
 */

#include "console.h"
#include "simpio.h"
#include "strlib.h"
using namespace std;


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


int main()
{
    string number = getLine("number of layers ?");

    int n = stringToReal(number);

    if (n==0){
        n = 6;
    }

    starPatterns(n);

    return 0;
}
