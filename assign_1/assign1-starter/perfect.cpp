/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "console.h"
#include <iostream>
#include <cmath>
#include "testing/SimpleTest.h"
using namespace std;

/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}


long smarterSum(long n) {
    long total = 0;
    for (long divisor = 2; divisor < sqrt(n); divisor++) {
        if (n % divisor == 0) {
            total += divisor + n /divisor;             //一次性计算除数与成对因子的和
        }
    }

    if (n > 1) {                                      // n > 1时，1是divisor
        total+=1;
        if (sqrt(n) == int(sqrt(n))){                 // 开方为整数时，开方数是divisor
            total+=sqrt(n);
        }
    }
    return total;
}


bool isPerfectSmarter(long n) {
    return (n != 0) && (n == smarterSum(n));
}


void findPerfectsSmarter(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}




// return 第n个完全数;  Principle: if the Mersenne number m=2^k-1 is prime --> 2^(k-1)*(2^k-1) is a perfect number
long findNthPerfectEuclid(long n) {
    // 初始化计数器
    long count=0;
    long k=1;

    // 循环计算到第n个完全数
    while(true) {
        //Mersenne number m
        long m = pow(2,k)-1;
        // 判断 m 是否为素数
        if (smarterSum(m) == 1){
            // 若为素数,则为完全数
            count++;
            //若计算到第n个完全数,return
            if (count == n){
                return pow(2,k-1)*(pow(2,k)-1);
            }
        }

        k++;
    }

    return 0;
}


/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(10000, findPerfects(10000));
    TIME_OPERATION(20000, findPerfects(20000));
    TIME_OPERATION(40000, findPerfects(40000));
}

PROVIDED_TEST("Confirm smarterSum of small inputs") {
    EXPECT_EQUAL(smarterSum(1), 0);
    EXPECT_EQUAL(smarterSum(6), 6);
    EXPECT_EQUAL(smarterSum(12), 16);
}

// TODO: add your student test cases here

STUDENT_TEST(" Q2: One minute "){
    TIME_OPERATION(20000, findPerfects(20000));
    TIME_OPERATION(40000, findPerfects(40000));
    TIME_OPERATION(80000, findPerfects(80000));
    TIME_OPERATION(160000, findPerfects(160000));
}

STUDENT_TEST(" Q3: amount of work "){
    TIME_OPERATION(10, findPerfects(10));
    TIME_OPERATION(1000, findPerfects(1000));
    TIME_OPERATION(2000, findPerfects(2000));
}


STUDENT_TEST("Q5: Negative numbers are not perfect") {
    EXPECT(!isPerfect(-1));
    EXPECT(!isPerfect(-20));
}
// 当 isPerfect 输入负值 n 时， divisorSum 返回值为 0, 二者永不相等，isPerfect返回false


STUDENT_TEST("Q6: Confirm smarterSum") {
    EXPECT_EQUAL(divisorSum(-1), smarterSum(-1));
    EXPECT_EQUAL(divisorSum(1), smarterSum(1));
    EXPECT_EQUAL(divisorSum(3), smarterSum(3));
    EXPECT_EQUAL(divisorSum(24), smarterSum(24));
    EXPECT_EQUAL(divisorSum(25), smarterSum(25));
    EXPECT_EQUAL(divisorSum(28), smarterSum(28));
    EXPECT_EQUAL(divisorSum(36), smarterSum(36));
}


STUDENT_TEST("Q7: Time trials of findPerfectsSmarter on doubling input sizes") {
    TIME_OPERATION(10000, findPerfectsSmarter(10000));
    TIME_OPERATION(20000, findPerfectsSmarter(20000));
    TIME_OPERATION(40000, findPerfectsSmarter(40000));
    //TIME_OPERATION(33550340, findPerfectsSmarter(33550340));
}


STUDENT_TEST("Q9: Confirm findNthPerfectEuclid") {
    // EXPECT(isPerfect(findNthPerfectEuclid(1)));
    EXPECT_EQUAL(findNthPerfectEuclid(1), 6);
    EXPECT_EQUAL(findNthPerfectEuclid(2), 28);
    EXPECT_EQUAL(findNthPerfectEuclid(3), 496);
    EXPECT_EQUAL(findNthPerfectEuclid(4), 8128);
    EXPECT_EQUAL(findNthPerfectEuclid(5), 33550336);
}

/*
STUDENT_TEST("Q10: Time trials of findPerfectsSmarter on doubling input sizes") {
    TIME_OPERATION(1, findNthPerfectEuclid(1));
    TIME_OPERATION(2, findNthPerfectEuclid(2));
    TIME_OPERATION(3, findNthPerfectEuclid(3));
    TIME_OPERATION(4, findNthPerfectEuclid(4));
    TIME_OPERATION(5, findNthPerfectEuclid(5));
    //TIME_OPERATION(33550340, findPerfectsSmarter(33550340));
}
*/
