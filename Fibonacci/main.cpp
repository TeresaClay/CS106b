/*
 * File: main.cpp
 * --------------
 * Blank C++ project configured to use Stanford cslib and Qt
 */

#include <iostream>
#include "console.h"
#include "simpio.h"
using namespace std;

// 兔子生三对幼崽后死亡，寿命为4个月
int fibDeath(int month) {
    int lifeMonth = 4;

    if(month < 2) {                                     // base case (star value)
        return month;
    } else if (month <= lifeMonth ){                    // 无死亡
        return fibDeath(month-1)+fibDeath(month-2);
    } else if (month < 2*lifeMonth){                    // 死亡一对
        return fibDeath(month-1)+fibDeath(month-2)-1;
    } else {
        return fibDeath(month-1)+fibDeath(month-2)-2;   // 死亡两对
    }
}


// 兔子没有死亡
int fib(int month) {
    if (month < 2) {
        return month;
    } else {
        return fib(month-1)+fib(month-2);
    }
}


int main()
{
    // Month从零开始,输入的数字为月初; 例如 3 表示三月初，相当与二月末
    int maxMonth = getInteger("Enter the month: ");

    cout <<"兔子没有死亡情况(月初兔子的对数，相当于上个月月末的对数; 从零月开始): " <<endl;
    for (int i=0; i <= maxMonth; i++){
        cout << fib(i) << " ";
    }

    cout << endl;
    cout << "兔子生三对幼崽后死亡，即寿命为4个月(月初兔子的对数，相当于上个月月末的对数; 从零月开始): " <<endl;
    for (int i=0; i <= maxMonth; i++){
        cout << fibDeath(i) << " ";
    }

    return 0;

}
