#include "SudokuDefinitions.h"
#include <stdbool.h>
#include <stdio.h>

// grid - 9x9 int array to hold contents of each cell in the sudoku board
extern int grid[9][9];
// given - 9x9 boolean array to hold the type of each cell (true = a given number, false = a penciled number)
extern bool given[9][9];
// correct - 9x9 boolean array to hold the validity of each cell (true = correct)
extern bool correct[9][9];
// solved - int value to keep track of state of the board (WRONG,CHECKED,CORRECT)
extern int solved;

// SudokuSolver - contains the following functions to handle all validations and calculations
bool genSolution(int row, int col);                          // resolves the board with recursive backtracking
bool markSolution(int row, int col);                         // marks incorrect cells with recursive backtracking
void getNumSolutions(int row, int col, int *count, int max); // calculates the number of solutions of the current board (up to the max) with recursive backtracking
bool isValidShallow(int row, int col, int num);              // does a shallow check of the cell (verifies it is unique within the row, column, and box)
bool isValidDeep(int row, int col, int num);                 // does a deep check of the cell (verifies there is at least one solution)

// resolves the board with recursive backtracking
bool genSolution(int row, int col) {
    //base case: reached the end of the board -> solved
    if (row == 8 && col == 9) {
        return true;
    }

    //if column overflow, move to next row
    if (col == 9) {
        col = 0;
        row += 1;
    }

    if (!given[row][col]) {
        //if cell is empty, try each number 1-9
        for (int i = 0; i < 9; i++) {
            //check if setting this will immediately break the puzzle
            if (isValidShallow(row, col, i + 1)) {
                //assume temporarily this number is right
                grid[row][col] = i + 1;

                //recursively check the next cell
                if (genSolution(row, col + 1)) {
                    //if found a solution, stop the recursion
                    return true;
                }
            }
        }

        //if didn't find a solution, reset the cell
        grid[row][col] = EMPTY;

        //return to parent for backtracking
        return false;
    } else {
        //if cell is already filled, skip it
        return genSolution(row, col + 1);
    }
}

// marks incorrect cells with recursive backtracking
bool markSolution(int row, int col) {
    //base case: reached the end of the board -> solved
    if (row == 8 && col == 9) {
        return true;
    }

    //if column overflow, move to next row
    if (col == 9) {
        col = 0;
        row += 1;
    }

    if (!given[row][col]) {
        //save initial number
        int prev = grid[row][col];

        //if cell is not given, try each number 1-9
        for (int i = 0; i < 9; i++) {
            //check if setting this will immediately break the puzzle
            if (isValidShallow(row, col, i + 1)) {
                //assume temporarily this number is right
                grid[row][col] = i + 1;

                //recursively check the next cell
                if (markSolution(row, col + 1)) {
                    //if found a solution, compare to the initial value
                    if (prev != i + 1) {
                        //if not correct, update correct array and revert cell
                        correct[row][col] = false;
                        solved = WRONG;
                    }
                    grid[row][col] = prev;
                    return true;
                }
            }
        }

        //if didn't find a solution, reset the cell
        grid[row][col] = EMPTY;

        //return to parent for backtracking
        return false;
    } else {
        //if cell is already filled, skip it
        return markSolution(row, col + 1);
    }
}

// calculates the number of solutions of the current board (up to the max) with recursive backtracking
void getNumSolutions(int row, int col, int *count, int max) {
    //base case: reached the end of the board -> solved
    if (row == 8 && col == 9) {
        //increase solution counter
        *count += 1;
        return;
    }

    //if column overflow, move to next row
    if (col == 9) {
        col = 0;
        row += 1;
    }

    if (grid[row][col] == EMPTY) {
        //if cell is empty, try each number 1-9
        for (int i = 0; i < 9; i++) {
            //check if setting this will immediately break the puzzle
            if (isValidShallow(row, col, i + 1)) {
                //assume temporarily this number is right
                grid[row][col] = i + 1;

                //recursively check the next cell
                getNumSolutions(row, col + 1, count, max);

                //reset the cell
                grid[row][col] = EMPTY;

                //if reached the maximum solution count, stop the recursion
                if (*count >= max) {
                    return;
                }
            }
        }
    } else {
        //if cell is already filled, skip it
        getNumSolutions(row, col + 1, count, max);
    }
}

// does a shallow check of the cell (verifies it is unique within the row, column, and box)
bool isValidShallow(int row, int col, int num) {
    //check for duplicate values in the same row and column
    for (int i = 0; i < 9; i++) {
        if (col != i && grid[row][i] == num)
            return false;
        if (row != i && grid[i][col] == num) {
            return false;
        }
    }

    //calculate the first row and column in the cell's box
    int startRow = row / 3 * 3;
    int startCol = col / 3 * 3;

    //check for duplicates in the same box
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (!(startRow + r == row && startCol + c == col) && grid[startRow + r][startCol + c] == num)
                return false;
        }
    }

    //cell passed shallow tests
    return true;
}

// does a deep check of the cell (verifies there is at least one solution)
bool isValidDeep(int row, int col, int num) {
    //do shallow check first
    if (!isValidShallow(row, col, num))
        return false;

    //do deep check if passed shallow check
    //save current value of the grid
    int previous = grid[row][col];

    //assume temporarily the cell was updated
    grid[row][col] = num;

    //verify there is at least one solution with this assumption
    int count = 0;
    getNumSolutions(0, 0, &count, 1); //will return 0 or 1 because set max of 1

    //reset the grid value
    grid[row][col] = previous;

    //return true if there was at least one solution
    return count == 1;
}