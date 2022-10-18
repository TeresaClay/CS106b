/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
#include <queue>
using namespace std;

// 辅助函数,判断 Queue是否排序
void isQueueSort(Queue<int> a){

    for (int i=0; i < a.size() -1; i++)
    {
        if (a.dequeue() > a.dequeue())
            error("Queue 排序错误");
    }

}

// 辅助函数,合并两个 Queue
Queue<int> mergeQueue(Queue<int> a, Queue<int> b){
    while (!b.isEmpty()){
        a.enqueue(b.dequeue());
    }
    return a;
}


// 归并两个独立的有序序列
Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {

    // 判断 Queue是否排序
    isQueueSort(a);
    isQueueSort(b);

    Queue<int> result;
    // 左右都有数据时，取出较小值，存入结果
    while (!a.isEmpty() && !b.isEmpty()){
        if (a.peek() < b.peek())
            result.enqueue(a.dequeue());
        else
            result.enqueue(b.dequeue());
    }
    // 任何一方数据取完，将另一方剩余数据一次性存入结果
    // 合并通过辅助函数mergeQueue实现的，是否有更好的方法？
    if (a.isEmpty())
         result = mergeQueue(result,b);
    else if (b.isEmpty())
         result = mergeQueue(result,a);

    return result;
}

/************************************************************************************************/


/*
 * This function assumes correct functionality of the previously
 * defined binaryMerge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}


/***********************************************************************************************/

Queue<int> recMultiMergeRec(const Vector<Queue<int>>& all, const int &start, const int & end) {

    // base case
    if ( end - start ==1 )
        return all[start];

    else {
        int mid = (start + end) / 2;
        Queue<int> leftResult = recMultiMergeRec(all, start, mid);
        Queue<int> rightResult = recMultiMergeRec(all, mid, end);
        return binaryMerge(leftResult, rightResult);
    }

}


Queue<int> recMultiMerge(Vector<Queue<int>>& all) {

    return recMultiMergeRec(all, 0, all.size());

}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}

STUDENT_TEST("isQueueSort") {

    Queue<int> a = {1,2,3,4};
    EXPECT_NO_ERROR(isQueueSort(a));

    Queue<int> b = {1,1,3,4};
    EXPECT_NO_ERROR(isQueueSort(b));
    Queue<int> d = {2,1,3,4};
    EXPECT_ERROR(isQueueSort(d));
}

STUDENT_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{},
                             {},
                             {},
                             {},
                             {},
                             {}
                            };
    Queue<int> expected = {};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

STUDENT_TEST("recMultiMerge"){
    Vector<Queue<int>> all = {{0,1,2},{2,3,5}};
    Queue<int> result = {0,1,2,2,3,5};
    EXPECT_EQUAL(recMultiMerge(all),result);

}





