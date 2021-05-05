#include <stdbool.h>
#include <stdio.h>

#define EMPTY 0
extern int grid[9][9];

void getNumSolutions(int row, int col, int *count);
bool isValid(int row, int col, int num);

void getNumSolutions(int row, int col, int *count) {
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
            if (isValid(row, col, i + 1)) {
                grid[row][col] = i + 1;
                getNumSolutions(row, col + 1, count);
                grid[row][col] = EMPTY;
                if (*count > 100000) {
                    return;
                }
            }
        }
    } else {
        getNumSolutions(row, col + 1, count);
    }
}

bool isValid(int row, int col, int num) {
    for (int i = 0; i < 9; i++) {
        if (grid[row][i] == num)
            return false;
        if (grid[i][col] == num) {
            return false;
        }
    }
    int startRow = row / 3 * 3;
    int startCol = col / 3 * 3;
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (grid[startRow + r][startCol + c] == num)
                return false;
        }
    }
    return true;
}