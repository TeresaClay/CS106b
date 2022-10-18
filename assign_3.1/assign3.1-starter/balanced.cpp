/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

// 接收一个字符串参数并返回一个只包含括号操作符的字符串
string operatorsFrom(string str) {
    // base case
    if (str.empty()){
        return "";
    } else {
        // 如果str中头元素属于符号,输出值包括这个符号
        if(string("(,),[,],{,}").find(str[0]) != string::npos)
            return str[0]+operatorsFrom(str.substr(1));
        // 否则,输出值不包括这个符号
        else
            return operatorsFrom(str.substr(1));
        }
    }


bool operatorsAreMatched(string ops) {

    // base case
    if (ops.empty())
        return true;

    else {
        // 若字符串包括"()", "[]", or "{}"这样的子字符串，判断删除后剩下部分是否平衡
        if (stringContains(ops,"()"))
            return operatorsAreMatched(ops.erase(ops.find("()"),2));
        if (stringContains(ops,"[]"))
            return operatorsAreMatched(ops.erase(ops.find("[]"),2));
        if (stringContains(ops,"{}"))
            return operatorsAreMatched(ops.erase(ops.find("{}"),2));
    }

    return false;
}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    // 从输入中提取出括号操作符
    string ops = operatorsFrom(str);
    // 确定输入是否配对
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}


STUDENT_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("()[]{}"), "()[]{}");
    EXPECT_EQUAL(operatorsFrom("x[w])"), "[])");
}

STUDENT_TEST("operatorsAreMatched on simple example") {
    EXPECT(!operatorsAreMatched("ab()[]{}"));
}
