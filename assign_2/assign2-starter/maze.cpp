// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/*
 * a function that takes in a given maze, and a cur, and
 * returns a Set<gridLocation> of valid graidLocations that "cur" can move to
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;

    //  West: row -1
    GridLocation West = {cur.row-1, cur.col};
    if ( maze.inBounds(West)  && maze.get(West) == true) {
        neighbors.add(West);
    }

    //  East: row +1
    GridLocation East = {cur.row+1, cur.col};
    if ( maze.inBounds(East)  && maze.get(East) == true) {
        neighbors.add(East);
    }

    //  North: col +1
    GridLocation North = {cur.row, cur.col+1};
    if ( maze.inBounds(North)  && maze.get(North) == true) {
        neighbors.add(North);
    }

    //  Sourth: col - 1
    GridLocation Sourth = {cur.row, cur.col-1};
    if ( maze.inBounds(Sourth)  && maze.get(Sourth) == true) {
        neighbors.add(Sourth);
    }

    return neighbors;
}



/*
 * - 验证所给路径Stack<GridLocation>path可以顺利走出迷宫
 * - If the path is a valid solution, then this function
 *   should run to completion
 */
void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {
    // 出口位置
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    // 入口位置
    GridLocation mazeEntrance = {0,0};

    // 路径的终点必须位于迷宫出口
    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }

    // 用来存放已经有过的路径
    Set<GridLocation> visited;

    int l = path.size();
    for (int i = l-1 ; i >= 0; i--){
        // 取出栈顶
        GridLocation cur = path.pop();

        // 计算 generateValidMoves
        Set<GridLocation> validLocs = generateValidMoves(maze, cur);

        // 判断 peek 是否 在 validLocs
        if ( i > 0 && !validLocs.contains(path.peek()))
            error ("当前移动不符合规则");

        // 判断 cur 是否 在 visited
        if (visited.contains(cur))
            error ("当前位置已经走过");

        // 判断起点
        if (i==0 && cur != mazeEntrance)
            error ("路径的起点不在迷宫入口(左上角)");

        visited.add(cur);

    }

}


/*************************************************************************/

// 把Stack<GridLocation> 转化为 Set<GridLocation>
Set<GridLocation> stackToSet (Stack<GridLocation> path) {
    Set<GridLocation> results;
    while (!path.isEmpty()) {
        results.add(path.pop());
    }
    return results;
}



Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    // 画出迷宫地图
    MazeGraphics::drawGrid(maze);
    // 出口位置
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    Stack<GridLocation> path;

    // 创建一个空的队列(Queue)，用于添加路径
    Queue<Stack<GridLocation>> pathQueue;

    // 创建一个仅包含入口的路径
    path = {{0,0}};
    // 将此路径添加到队列中
    pathQueue.enqueue(path);

    while (!pathQueue.isEmpty()) {

        // 从队列中取出当前路径
        path = pathQueue.dequeue();
        MazeGraphics::highlightPath(path, "green", 100);
        // 判断当前路径是否已经达到出口
        if ( path.peek() == mazeExit)
            return path;

        // 否则根据当前路径的末端，找出所有可以继续前进的位置
        Set<GridLocation> neighborsSet = generateValidMoves(maze, path.peek());

        for (GridLocation elem : neighborsSet){

            // 判断是否出现回头现象; 辅助函数stackToSet: 把Stack<GridLocation>转化为Set<GridLocation>
            if ( ! stackToSet(path).contains(elem)) {
                Stack<GridLocation> tempPath = path;
                tempPath.push(elem);
                pathQueue.enqueue(tempPath);
            }
        }
    }

    return path;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

// TODO: add your test cases here

STUDENT_TEST("测试 generateValidMoves 函数 (1)") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    GridLocation cur = {0,0};
    Set<GridLocation> results = { {1,0}, {0,1} };
    EXPECT_EQUAL(generateValidMoves(maze, cur), results);

}

STUDENT_TEST("测试 generateValidMoves 函数 (2)") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    GridLocation cur = {15,7};
    Set<GridLocation> results;
    EXPECT_EQUAL(generateValidMoves(maze, cur), results);

}


STUDENT_TEST("测试 generateValidMoves 函数 (3)") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    GridLocation cur = {22,24};
    Set<GridLocation> results;
    EXPECT_EQUAL(generateValidMoves(maze, cur), results);

}

STUDENT_TEST("solveMaze on file 25x33") {
    Grid<bool> maze;
    readMazeFile("res/25x33.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}
