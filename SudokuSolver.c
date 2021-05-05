#include "SudokuDefinitions.h"
#include <stdbool.h>
#include <stdio.h>

#define EMPTY 0
extern int grid[9][9];

bool genSolution(int row, int col);
void getNumSolutions(int row, int col, int *count, int max);
bool isValidShallow(int row, int col, int num);
bool isValidDeep(int row, int col, int num);

bool genSolution(int row, int col) {
    if (row == 8 && col == 9) {
        return true;
    }
    if (col == 9) {
        col = 0;
        row += 1;
    }
    if (grid[row][col] == EMPTY) {
        for (int i = 0; i < 9; i++) {
            if (isValidShallow(row, col, i + 1)) {
                grid[row][col] = i + 1;
                if (genSolution(row, col + 1)) {
                    return true;
                }
            }
        }
        grid[row][col] = EMPTY;
        return false;
    } else {
        return genSolution(row, col + 1);
    }
}

void getNumSolutions(int row, int col, int *count, int max) {
    if (row == 8 && col == 9) {
        *count += 1;
        return;
    }
    if (col == 9) {
        col = 0;
        row += 1;
    }
    if (grid[row][col] == EMPTY) {
        for (int i = 0; i < 9; i++) {
            if (isValidShallow(row, col, i + 1)) {
                grid[row][col] = i + 1;
                getNumSolutions(row, col + 1, count, max);
                grid[row][col] = EMPTY;
                if (*count >= max) {
                    return;
                }
            }
        }
    } else {
        getNumSolutions(row, col + 1, count, max);
    }
}

bool isValidShallow(int row, int col, int num) {
    for (int i = 0; i < 9; i++) {
        if (col != i && grid[row][i] == num)
            return false;
        if (row != i && grid[i][col] == num) {
            return false;
        }
    }
    int startRow = row / 3 * 3;
    int startCol = col / 3 * 3;
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (!(startRow + r == row && startCol + c == col) && grid[startRow + r][startCol + c] == num)
                return false;
        }
    }
    return true;
}

bool isValidDeep(int row, int col, int num) {
    if (!isValidShallow(row, col, num))
        return false;
    int previous = grid[row][col];
    grid[row][col] = num;
    int count = 0;
    getNumSolutions(0, 0, &count, 1);
    grid[row][col] = previous;
    return count == 1;
}