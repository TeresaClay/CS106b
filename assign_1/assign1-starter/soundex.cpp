/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
using namespace std;

// Step6: 通过填充 0 或截掉多余部分，使得编码长度恰好为 4
string adjustStrlen(string s) {
    if (s.size() > 4)
        return s.substr(0,4);

    for (int i=s.size(); i < 4; i++){
        s +='0';
    }
    return s;
}


// Step5: 删除编码中所有的数字 0
string removeZero(string s) {
    string result="";
    for (int i = 0; i < s.length(); i++ ){
        if (s[i] != '0' )
            result += s[i];
    }
    return result;
}


// Step4: 姓氏名的第一个字母替换第一个数字，并转换为大写形式
string replaceString(string s, string name) {
    if ( s.size() && name.size())
        s[0] = toUpperCase(name[0]);

    return s;
}


// Step3: 合并相邻的重复数字
string coalesceString(string s) {
    string result ="";

    for (int i = 0; i < s.length(); i++ ){
        if (result.back() != s[i] ) {
            result += s[i];
        }
    }
    return result;
}


// Step2: 编码每个字母
string encodeString(string s) {
    string result = "";
    string S = toUpperCase(s);
    string zero = "AEIOUHWY";
    string one = "BFPV";
    string two = "CGJKQSXZ";
    string three = "DT";
    string four ="L";
    string five ="MN";
    string six = "R";

    for (int i = 0; i < S.length(); i++ ) {
        if (stringContains(zero,S[i])) {
            result.append("0");
            continue;
        }
        if (stringContains(one,S[i])) {
            result.append("1");
            continue;
        }
        if (stringContains(two,S[i])) {
            result.append("2");
            continue;
        }
        if (stringContains(three,S[i])) {
            result.append("3");
            continue;
        }
        if (stringContains(four,S[i])) {
            result.append("4");
            continue;
        }
        if (stringContains(five,S[i])) {
            result.append("5");
            continue;
        }
        if (stringContains(six,S[i])) {
            result.append("6");
            continue;
        }
    }
    return result;
}


// Step2: 编码每个字母; case方法不利于后续修改
/*
Vector<int> encodeString(string s) {
    Vector<int> codedName;
    string S = toUpperCase(s);
    for (int i=0; i < S.length(); i++ ) {
       switch (S[i]){
       case 'A': case 'E': case 'I': case 'O': case 'U': case 'H': case 'W': case 'Y':
           codedName.add(0);
           break;
       case 'B': case 'F': case 'P': case 'V':
           codedName.add(1);
           break;
       case 'C': case 'G': case 'J': case 'K': case 'Q': case 'S': case 'X': case 'Z':
           codedName.add(2);
           break;
       case 'D': case 'T':
           codedName.add(3);
           break;
       case 'L':
           codedName.add(4);
           break;
       case 'M': case 'N':
           codedName.add(5);
           break;
       case 'R':
           codedName.add(6);
           break;
       }
    }
    return codedName;
}
*/

/* Step1: This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * WARNING: The provided code is buggy!
 *
 * Use unit tests to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */
string removeNonLetters(string s) {
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}

/*
 *  顶层函数
 *  "Curie"    -> "C600"
 *  "O'Conner" -> "0256"
 *  ""         -> "0000"
 */
string soundex(string s) {
    string result;

    result = removeNonLetters(s);
    result = encodeString(result);
    result = coalesceString(result);
    result = replaceString(result,s);
    result = removeZero(result);
    result = adjustStrlen(result);

    return result;
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames


    while(true){
        string surname = getLine("Enter a surname (回车键退出): ");
        if (surname != ""){                             //
            // 计算输入姓的soundex code
            string surnameSoundex = soundex(surname);
            // 初始化结果
            Vector<string> result;
            // 计算数据库中每个姓的 soundex code
            for (int i=0; i < databaseNames.size(); i++){
                // 若数据库中姓与输入的姓有相同的 soundex code, 保存这个姓
                if ( soundex(databaseNames[i]) == surnameSoundex)
                    result += databaseNames[i];
            }

        result.sort();

        cout << "Soundex code is " << surnameSoundex << endl;
        cout << "Matches from database: " << result << endl;

        } else
            break;
    }

}

/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here

STUDENT_TEST("函数removeNonLetters的 bug") {
    EXPECT_EQUAL(removeNonLetters("_AB"),"AB");
    EXPECT_EQUAL(removeNonLetters(""),"");
    EXPECT_EQUAL(removeNonLetters("_,*"),"");
}

STUDENT_TEST("测试函数encodeString") {

    EXPECT_EQUAL(encodeString("Curie"), "20600");
    EXPECT_EQUAL(encodeString("OConner"), "0205506");
}

STUDENT_TEST("测试函数coalesceString") {

    EXPECT_EQUAL(coalesceString("20600"), "2060");
    EXPECT_EQUAL(coalesceString("0205506"), "020506");
    EXPECT_EQUAL(coalesceString("aab0330dd6"), "ab030d6");
}

STUDENT_TEST("测试函数replaceString") {

    EXPECT_EQUAL(replaceString("2060","bing"), "B060");
}

STUDENT_TEST("测试函数removeZero") {

    EXPECT_EQUAL(removeZero("2060"), "26");
    EXPECT_EQUAL(removeZero("0000"), "");
    EXPECT_EQUAL(removeZero("00ac3_6"), "ac3_6");
}


STUDENT_TEST("测试函数adjustStrlen") {

    EXPECT_EQUAL(adjustStrlen("26"), "2600");
    EXPECT_EQUAL(adjustStrlen(""), "0000");
    EXPECT_EQUAL(adjustStrlen("00ac3_6"), "00ac");
}

STUDENT_TEST("测试函数soundex") {

    EXPECT_EQUAL(soundex("Curie"), "C600" );
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
    EXPECT_EQUAL(soundex(""), "0000");

}
