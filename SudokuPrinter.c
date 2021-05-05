#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define EMPTY 0

extern int grid[9][9];
extern bool given[9][9];
extern bool valid;
extern int numEntered;

void printTitle();
void printWelcomeMessage();
void printEndMessage();
void printCommandErrorMessage();
void printPrompt();
void printGrid();
void printPanel(bool validated, double time_taken, int count);
void printNumSolutions(int count);
void printBold(char *str);
void printGray(char *str);
void printCyan(char *str);
void clear();

void printTitle() {
    clear();
    printf(" ------------ Sudoku Maker ------------ \n\n");
}

void printWelcomeMessage() {
    printTitle();
    printf("Welcome to Sudoku Maker!\n\n");
    printf("Using Sudoku Maker, you can create or solve any valid sudoku board.\n");
    printf("At any time, you may enter 'g' to generate a valid board, 'r' to restart, or 'e' to exit.\n");
    printf("To begin, enter the letter 's' for start.\n");
}

void printEndMessage() {
    printf("\nThank you for using Sudoku Maker!\n\n");
}

void printCommandErrorMessage() {
    printf("Sorry the command you entered isn't recognized.\n");
    printf("Please enter the correct command to continue, 'r' to restart, or 'e' to exit.\n");
}

void printPrompt() {
    printf("Please enter a cell followed by a number 0-9 (ie 'A1 1'): ");
}

void printGrid() {
    printf(" ");
    for (int i = 0; i < 9; i++) {
        printf("   %d", i + 1);
    }
    printf("\n");
    for (int row = 0; row < 9; ++row) {
        if (row % 3 == 0) {
            printBold("  +-----------+-----------+-----------+\n");
        } else {
            printBold("  |");
            for (int i = 0; i < 3; ++i) {
                printGray("---+---+---");
                printBold("|");
            }
            printBold("\n");
        }
        printf("%c ", row + 'A');
        printBold("|");
        for (int col = 0; col < 9; ++col) {
            int num = grid[row][col];
            if (num == EMPTY) {
                printf("   ");
            } else if (given[row][col]) {
                char *str;
                sprintf(str, " %d ", num);
                printCyan(str);
            } else {
                printf(" %d ", num);
            }
            if (col % 3 == 2) {
                printBold("|");
            } else {
                printGray("|");
            }
        }
        printf("\n");
    }
    printBold("  +-----------+-----------+-----------+\n");
}

void printPanel(bool validated, double time_taken, int count) {
    printTitle();
    printGrid();
    printNumSolutions(count);
    if (validated) {
        printf("It took %f seconds to check your board.\n", time_taken);
    }
    printPrompt();
}

void printNumSolutions(int count) {
    if (valid && count == 1) {
        printf("This is a valid sudoku board!\n");
    } else if (numEntered < 17) {
        printf("You need at least %d more numbers to make a valid sudoku board.\n", 17 - numEntered);
    } else if (!valid && count == 1) {
        printf("You've made a valid sudoku board!\n");
        valid = true;
    } else if (count == 0) {
        printf("This sudoku board is impossible to solve...\n");
    } else if (count <= 1000) {
        printf("There are %d solutions to this grid.\n", count);
    } else {
        printf("There are 1000+ solutions to this grid.\n");
    }
}

void printBold(char *str) {
    printf("\e[1m%s\e[0m", str);
}

void printGray(char *str) {
    printf("\e[90m%s\e[0m", str);
}

void printCyan(char *str) {
    printf("\e[36m%s\e\x1B[0m", str);
}
void clear() {
    system("clear");
}
