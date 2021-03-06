#include "SudokuDefinitions.h"
#include "SudokuSolver.c"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// grid - 9x9 int array to hold contents of each cell in the sudoku board
extern int grid[9][9];
// given - 9x9 boolean array to hold the type of each cell (true = a given number, false = a penciled number)
extern bool given[9][9];
// correct - 9x9 boolean array to hold the validity of each cell (true = correct)
extern bool correct[9][9];
// unique - boolean value to keep track of state of board (true = there is a unique solution, false = there are multiple solutions)
extern bool unique;
// solved - int value to keep track of state of the board (WRONG,CHECKED,CORRECT)
extern int solved;
// numGivens - int value to keep track of the number of givens inputted so far (at least 17 are needed for a unique solution)
extern int numGivens;
// inHelp - boolean value used to toggle the help page
extern bool inHelp;
// pencilMode - if true, all inputs are treated are no longer treated as givens
extern bool pencilMode;

// SudokuPrinter - contains the following functions to print the UI and send messages to the user
void printTitle();                                        // prints header
void printWelcomeMessage();                               // prints start up message
void printEndMessage();                                   // prints exit message
void printHelpMessage();                                  // prints help page
void printCommandErrorMessage();                          // prints error message for when input is not recognized
void printUnsolvableMessage();                            // prints error message for when user attempts to use solver on non-unique board
void printInvalidInputMessage(int row, int col, int num); // prints error message for when user attempts to set a given that will break the puzzle
void printUnableToUndoMessage();                          // prints error message for when user attempts to undo when there are no moves to undo
void printCantOverrideGivenMessage();                     // prints error message for when user attempts to change given cell in pencil mode
void printUnableToEnterPencilModeMessage();               // prints error message for when user attempts to enter pencil mode for a non-unique board
void printUnableToCheckMessage();                         // prints error message for when user attempts to check the board while not in pencil mode
void printPrompt();                                       // prints default prompt message to enter cell(s)
void printGrid();                                         // prints the sudoku board
void printPanel();                                        // prints the default panel, including the title, grid, number of solutions, elapsed time, and prompt
void printNumSolutions(int count);                        // prints blurb about solutions depending on count
void printSolvingState();                                 // prints blurb about solution state of the board
void printBold(char *str);                                // prints a bold white string
void printGray(char *str);                                // prints a thin grey string
void printBlue(char *str);                                // prints a bold blue string
void printGreen(char *str);                               // prints a thin green string
void printYellow(char *str);                              // prints a thin yellow string
void printRed(char *str);                                 // prints a bold red string
void printError(char *str);                               // prints a thin red string
void clear();                                             // clears the console

// prints header
void printTitle() {
    clear();
    printBold(" ------------ Sudoku Maker ------------\n");
    if (pencilMode && !inHelp) {
        printGray("            ~ Pencil Mode ~");
    }
    printf("\n\n");
}

// prints start up message
void printWelcomeMessage() {
    printTitle();
    printBlue("Welcome to Sudoku Maker!\n\n");
    printf("Using Sudoku Maker, you can create or solve any valid sudoku board.\n");
    printf("To see a list of commands, you may enter 'h' for help.\n");
    printBlue("\nTo begin, hit enter.\n");
}

// prints exit message
void printEndMessage() {
    printBlue("\nThank you for using Sudoku Maker!\n\n");
}

// prints help page
void printHelpMessage() {
    //update inHelp toggle
    inHelp = true;

    printTitle();
    printBlue("The following are single letter commands that you can enter at any time:\n\n");
    printf("c");
    printGray(" - to check the current board and highlight the incorrect cells (only in pencil mode)\n");
    printf("e");
    printGray(" - to exit the program\n");
    printf("g");
    printGray(" - to automatically generate a valid board\n");
    printf("h");
    printGray(" - to enter/exit this help screen\n");
    printf("m");
    printGray(" - to exit pencil mode (clears your pencil marks)\n");
    printf("p");
    printGray(" - to enter pencil mode where you can attempt to solve the board yourself\n");
    printf("r");
    printGray(" - to reset or clear the board\n");
    printf("s");
    printGray(" - to solve the board (board must be valid)\n");
    printf("u");
    printGray(" - to undo the last cell assignment you made\n");
    // printf("*note - to erase a cell, set enter the number 0 (ie 'A1 0')\n");
}

// prints error message for when input is not recognized
void printCommandErrorMessage() {
    printError("Sorry, the command you entered isn't recognized.\n");
    printf("Please enter the correct command to continue or enter 'h' for help.\n");
}

// prints error message for when user attempts to use solver on non-unique board
void printUnsolvableMessage() {
    printError("\nSorry, you must enter a valid sudoku board (with exactly one solution) to use the solve command.\n");
}

// prints error message for when user attempts to set a given that will break the puzzle
void printInvalidInputMessage(int row, int col, int num) {
    //convert 0-indexed row and column into user friendly values
    char rowChar = row + 'A';
    col += 1;

    char *str = malloc(sizeof(char) * 45);
    sprintf(str, "Sorry, your input <%c%d %d> was invalid.\n", rowChar, col, num);
    printError(str);
    free(str);
    printPrompt();
}

// prints error message for when user attempts to undo when there are no moves to undo
void printUnableToUndoMessage() {
    printError("\nSorry, you have no moves to undo.\n");
}

// prints error message for when user attempts to change given cell in pencil mode
void printCantOverrideGivenMessage() {
    printError("Sorry, you can't update a given cell in pencil mode.\n");
}

// prints error message for when user attempts to enter pencil mode for a non-unique board
void printUnableToEnterPencilModeMessage() {
    printError("Sorry, you can only enter pencil mode when your board is valid and has a unique solution.\n");
}

// prints error message for when user attempts to check the board while not in pencil mode
void printUnableToCheckMessage() {
    printError("\nSorry, you can only check the board when you are in pencil mode.\n");
}

// prints default prompt message to enter cell(s)
void printPrompt() {
    printf("Please enter a command or 1+ cells separated by spaces (ie 'A1 1 B2 2'): ");
    fflush(stdout);
}

// prints the sudoku board
void printGrid() {
    //spacing for top row
    printf(" ");

    //print column numbers
    for (int i = 0; i < 9; i++) {
        printf("   %d", i + 1);
    }
    printf("\n");

    //print grid
    for (int row = 0; row < 9; ++row) {
        //print top border of each row
        if (row % 3 == 0) {
            //if at the top row of a row group, print bold separator
            printBold("  +-----------+-----------+-----------+\n");
        } else {
            //if an internal row of a row group, print thin separator
            printBold("  |");
            for (int i = 0; i < 3; ++i) {
                printGray("---+---+---");
                printBold("|");
            }
            printf("\n");
        }

        //print row letter
        printf("%c ", row + 'A');

        //print left border
        printBold("|");

        //print each cell in the row
        for (int col = 0; col < 9; ++col) {
            int num = grid[row][col];

            //format string to print in proper color
            char *str = malloc(sizeof(char) * 3);
            sprintf(str, " %d ", num);

            if (num == EMPTY) {
                //if cell is not set, print empty space
                printf("   ");
            } else if (given[row][col]) {
                //if cell is a given, print the number in blue
                printBlue(str);
            } else if (!correct[row][col]) {
                //if cell is incorrect, print the number in red
                printRed(str);
            } else {
                //if cell is not a give, print the number in white
                printGray(str);
            }
            free(str);

            if (col % 3 == 2) {
                //if at the rightmost column of a column group, print bold separator
                printBold("|");
            } else {
                //if an internal column of a column group, print thin separator
                printGray("|");
            }
        }

        printf("\n");
    }

    //print bottom border
    printBold("  +-----------+-----------+-----------+\n\n");
}

// prints the default panel, including the title, grid, number of solutions, elapsed time, and prompt
void printPanel() {
    //update inHelp as printing the panel exits the help page
    inHelp = false;

    //declare variables to calculate elapsed time
    clock_t t;
    double time;

    //validated = true if the number of solutions was calculated
    bool validated = false;

    //count = number of solutions
    int count = 0;

    //only calculate number of solutions if there are at least 17 givens (17+ are needed for a unique solution)
    if (numGivens >= 17 && !pencilMode) {
        //set t to time when started calculation
        t = clock();

        getNumSolutions(0, 0, &count, MAX_SOLUTIONS);

        //set t to elapsed time for calculation
        t = clock() - t;

        //convert time into seconds
        time = ((double)t) / CLOCKS_PER_SEC;

        validated = true;
    }

    //print panel
    printTitle();
    printGrid();

    if (!pencilMode) {
        //print blurb about solutions depending on count
        printNumSolutions(count);

        //if calculation occurred, print elapsed time for calculation
        if (validated) {
            char *str = malloc(sizeof(char) * 50);
            sprintf(str, "It took %.5f seconds to check your board.\n", time);
            printGray(str);
            free(str);
        }
    } else {
        //print blurb about solved state of board
        int errors = 0;
        int emptyCells = 0;
        getNumErrors(&errors, &emptyCells);
        printSolvingState(errors, emptyCells);
    }

    //prompt user for cell input
    printPrompt();
}

// prints blurb about solutions depending on count
void printNumSolutions(int count) {
    if (unique && count == 1) {
        //sudoku board was already valid
        printGreen("This is a valid sudoku board!\n");
    } else if (numGivens < 17) {
        //not enough givens for a valid solution
        char *str = malloc(sizeof(char) * 70);
        sprintf(str, "You need at least %d more numbers to make a valid sudoku board.\n", 17 - numGivens);
        printYellow(str);
        free(str);
    } else if (!unique && count == 1) {
        //sudoku board just became valid
        printGreen("You've made a valid sudoku board!\n");
        unique = true;
    } else if (count == 0) {
        printRed("This sudoku board is impossible to solve...\n");
    } else if (count < MAX_SOLUTIONS) {
        char *str = malloc(sizeof(char) * 50);
        sprintf(str, "There are %d solutions to this grid.\n", count);
        printYellow(str);
        free(str);
    } else {
        //number of solutions exceeded maximum
        char *str = malloc(sizeof(char) * 50);
        sprintf(str, "There are %d+ solutions to this grid.\n", MAX_SOLUTIONS);
        printYellow(str);
        free(str);
    }
}

//prints blurb about solution state of the board
void printSolvingState(int errors, int emptyCells) {
    if (solved == CORRECT) {
        printGreen("Congrats! You've solved this board!\n");
    } else if (solved == WRONG) {
        printError("Sorry, it doesn't look like you have the right answer.\n");
        char *str = malloc(sizeof(char) * 50);
        sprintf(str, "There are %d errors and %d empty cells left.\n", errors, emptyCells);
        printGray(str);
        free(str);
    }
}

// prints a bold white string
void printBold(char *str) {
    printf("\e[1m%s\e[0m", str);
}

// prints a thin grey string
void printGray(char *str) {
    printf("\e[90m%s\e[0m", str);
}

// prints a bold blue string
void printBlue(char *str) {
    printf("\e[1;34m%s\e[0m", str);
}

// prints a thin green string
void printGreen(char *str) {
    printf("\e[0;32m%s\e[0m", str);
}

// prints a thin yellow string
void printYellow(char *str) {
    printf("\e[0;33m%s\e[0m", str);
}

// prints a bold red string
void printRed(char *str) {
    printf("\e[1;31m%s\e[0m", str);
}

// prints a thin red string
void printError(char *str) {
    printf("\e[31m%s\e[0m", str);
}

// clears the console
void clear() {
    system("clear");
}
