// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"
#include "simpio.h"
using namespace std;

// 返回一个“纯净版”的字符串
string cleanToken(string s)
{  

    if (s.empty())
        return "";

    // 将字符串转换为小写
    s = toLowerCase(s);

    // 去除字符串首的标点符号: 找到第一个字母的位置，删除之前的符号
    for (int i=0; i < s.size(); i++){
        if (isalpha(s[i])){
            s = s.erase(0,i);
            break;
        }
    }

    // 去除字符串尾的标点符号: 找到最后字母的位置，删除之后的符号
    for (int i=s.size()-1; i < s.size(); i--){
        if (isalpha(s[i])){
            s = s.erase(i+1,s.size()-1-i);
            break;
        }
    }

    for (int i=0; i < s.size(); i++){
        // 若字符串包含字母, return s
        if (isalpha(s[i])){
            return s;
        }
    }

    // 若字符串不包含字母
    return "";

}

// 提取出所有不重复的字符串，并以集合的形式返回
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    //拆分成一个个独立的子字符串
    Vector<string> input = stringSplit(text, " ");

    for (string elem: input) {

        string word = cleanToken(elem);

        if (!word.empty())
            tokens.add(word);
    }

    return tokens;
}

/*
 * 从数据库文件读取内容，并将其处理为反向索引的形式
 * string dbfile  ---> 网页数据库文件的名字
 * Map<string, Set<string>>& index ---> 反向索引数据
 * return ---> 数据库文件中处理过的网页数量
 */
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{

    // 参考readMazeFile: 把文件打开，并把内容存入一个 Vector
    ifstream in;

    if (!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);

    Vector<string> lines;
    readEntireFile(in, lines);

    // 遍历内容行
    for (int i=1; i < lines.size(); i+=2){
        // 提取出所有不重复的字符串
        Set<string> tokens = gatherTokens(lines[i]);
        // 遍历
        for (auto elem : tokens){
            // Map<string, Set<string>> -> 字符串 网址
            index[elem].add(lines[i-1]);
        }
    }

    // 返回网页数量
    return (lines.size())/2;
}

/*
 * 查询关键词 (单个 or 组合)
 * string query: 需要查询的字符串
 */
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;

    //拆分复合搜索关键词
    Vector<string> tokens = stringSplit(query, " ");

    for (string elem : tokens) {
        // 没有使用修饰符连接关键词
        if (isalpha(elem[0])){
            // 搜索结果将是每个关键词匹配项的并集
            result.unionWith(index.get(cleanToken(elem)));

            // 组合关键词中使用 +
        } else if (elem[0] == '+'){
            // 搜索结果将是每个关键词匹配项的交集
            result.intersect(index.get(cleanToken(elem)));

            // 组合关键词中使用 -
        } else if (elem[0] == '-'){
            // 去除 - 号后面关键词的匹配项
            result.difference(index.get(cleanToken(elem)));
        }
    }

    return result;
}


void searchEngine(string dbfile)
{
    // 根据数据库文件内容，构建一个反向索引
    Map<string, Set<string>> index;
    int n = buildIndex(dbfile, index);

    // 打印出构建反向索引用到的网页数量以及所有页面中包含了多少独一无二的单词
    cout << "Indexed " << n << " pages containing " << index.size() << " unique terms" << endl;


    while(true){
        //进入一个循环，提示用户输入查询关键词
        string query = getLine("Enter query sentence (Enter to quit)): ");

        if (query != "") {
            // 对于每一个查询关键词，找出所有匹配页面
            Set<string> URLs = findQueryMatches(index, query);
            //并打印出 URLs
            cout << "Found " << URLs.size() << " matching pages" << endl;
            cout << URLs << endl;

        // 当用户输入空字符串时, 程序结束
        } else
            break;
    }
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// TODO: add your test cases here
STUDENT_TEST("测试函数cleanToken") {
    EXPECT_EQUAL(cleanToken("*_\?"), "");
    EXPECT_EQUAL(cleanToken(""), "");
    EXPECT_EQUAL(cleanToken("<<blue>>"), "blue");
}


STUDENT_TEST("Time test on buildIndex from website.txt, 32 pages") {
    Map<string, Set<string>> index;
    TIME_OPERATION(32, buildIndex("res/website.txt", index));
    //EXPECT_EQUAL(index.size(), 11);
    //EXPECT(index.containsKey("fish"));
}
