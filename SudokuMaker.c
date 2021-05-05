#include "SudokuPrinter.c"
#include "SudokuSolver.c"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define EMPTY 0

int grid[9][9];
bool given[9][9];
bool valid;
int numEntered;
bool inHelp = false;

void handleInput();
void handleCommand(char command, bool *stop);
void handleCellInput(char *cell);
void showPanel();
void resetGrid();
void solveGrid();
void genGrid();
void updateGrid(int row, int col, int num, bool isGiven);

int main(void) {
    printWelcomeMessage();

    getchar();

    showPanel();

    handleInput();

    return 0;
}

void handleInput() {
    bool stop = false;
    while (!stop) {
        char *input = malloc(2);

        scanf("%s", input);

        if (strlen(input) == 1) {
            handleCommand(input[0], &stop);
        } else if (strlen(input) == 2) {
            handleCellInput(input);
        } else {
            printCommandErrorMessage();
        }
    }
}

void handleCommand(char command, bool *stop) {
    if (command == 'e') {
        printEndMessage();
        *stop = true;
    } else if (command == 'r') {
        resetGrid();
        showPanel();
    } else if (command == 'g') {
        resetGrid();
        genGrid();
        showPanel();
    } else if (command == 'h') {
        if (inHelp) {
            showPanel();
        } else {
            printHelpMessage();
        }
        inHelp = !inHelp;
    } else if (command == 's') {
        solveGrid();
    } else if (command != '\n') {
        printCommandErrorMessage();
    }
}

void handleCellInput(char *cell) {
    char rowChar;
    int col, num;
    sscanf(cell, "%c%d", &rowChar, &col);
    int row = rowChar - 'A';
    col--;
    scanf("%d", &num);
    if (row < 0 || row > 8 || col < 0 || col > 8 || num < EMPTY || num > 9) {
        printCommandErrorMessage();
    } else {
        updateGrid(row, col, num, true);
        showPanel();
    }
}

void showPanel() {
    clock_t t;
    double time_taken;
    bool validated = false;

    int count = 0;
    if (numEntered >= 17) {
        t = clock();
        getNumSolutions(0, 0, &count);
        t = clock() - t;
        time_taken = ((double)t) / CLOCKS_PER_SEC;
        validated = true;
    }

    printPanel(validated, time_taken, count);
}

void resetGrid() {
    valid = false;
    numEntered = 0;
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            grid[row][col] = EMPTY;
            given[row][col] = false;
        }
    }
}

void solveGrid() {
    if (valid) {
        genSolution(0, 0);
        showPanel();
    } else {
        printUnsolvableMessage();
    }
}

void genGrid() {
    grid[0][0] = 8;
    grid[0][6] = 7;
    grid[0][7] = 4;
    grid[1][2] = 1;
    grid[1][5] = 5;
    grid[1][8] = 6;
    grid[2][1] = 9;
    grid[2][2] = 2;
    grid[2][4] = 8;
    grid[2][5] = 4;
    grid[3][3] = 9;
    grid[3][7] = 2;
    grid[4][2] = 7;
    grid[4][7] = 6;
    grid[5][1] = 5;
    grid[5][2] = 3;
    grid[5][5] = 8;
    grid[5][8] = 7;
    grid[6][0] = 5;
    grid[6][8] = 1;
    grid[7][0] = 6;
    grid[7][3] = 1;
    grid[7][4] = 4;
    grid[7][7] = 5;
    grid[8][1] = 1;
    grid[8][5] = 3;
    grid[8][6] = 6;
    grid[8][8] = 4;
    numEntered = 28;
    given[0][0] = true;
    given[0][6] = true;
    given[0][7] = true;
    given[1][2] = true;
    given[1][5] = true;
    given[1][8] = true;
    given[2][1] = true;
    given[2][2] = true;
    given[2][4] = true;
    given[2][5] = true;
    given[3][3] = true;
    given[3][7] = true;
    given[4][2] = true;
    given[4][7] = true;
    given[5][1] = true;
    given[5][2] = true;
    given[5][5] = true;
    given[5][8] = true;
    given[6][0] = true;
    given[6][8] = true;
    given[7][0] = true;
    given[7][3] = true;
    given[7][4] = true;
    given[7][7] = true;
    given[8][1] = true;
    given[8][5] = true;
    given[8][6] = true;
    given[8][8] = true;
    valid = true;
}

void updateGrid(int row, int col, int num, bool isGiven) {
    if (num == EMPTY) {
        numEntered -= 1;
    } else if (grid[row][col] == EMPTY) {
        numEntered += 1;
    }
    grid[row][col] = num;
    given[row][col] = isGiven;
}