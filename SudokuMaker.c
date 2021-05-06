#include "SudokuDefinitions.h"
#include "SudokuPrinter.c"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// grid - 9x9 int array to hold contents of each cell in the sudoku board
int grid[9][9];
// given - 9x9 boolean array to hold the type of each cell (true = a given number, false = a penciled number)
bool given[9][9];
// unique - boolean value to keep track of state of board (true = there is a unique solution, false = there are multiple solutions)
bool unique;
// numGivens - int value to keep track of the number of givens inputted so far (at least 17 are needed for a unique solution)
int numGivens;
// inHelp - boolean value used to toggle the help page
bool inHelp = false;

// SudokuMaker - contains the following functions to handle input and manipulate the sudoku board
void handleInput();                                       // handles all of the user input
void handleCommand(char command, bool *stop);             // handles single letter commands
void handleCellInput(char *cell);                         // handles cell location and number inputs
void resetGrid();                                         // clears the board
bool solveGrid();                                         // solves board if the board is unique
void genGrid();                                           // generates a valid board
bool updateGrid(int row, int col, int num, bool isGiven); // updates a cell in the board

int main(void) {
    printWelcomeMessage();

    //wait for 'enter' key to continue
    getchar();

    //print default screen
    printPanel();

    //start input loop
    handleInput();

    return 0;
}

// handles all of the user input
void handleInput() {
    bool stop = false;

    //loop until user enters exit command 'e'
    while (!stop) {
        //allocate space for the input
        //the largest word should be two characters (for the cell location)
        char *input = malloc(2);

        scanf("%s", input);

        //determine type of input by its length
        if (strlen(input) == 1) {
            //one letter = command
            //pass stop in case exit command is called
            handleCommand(input[0], &stop);
        } else if (strlen(input) == 2) {
            //two letter = cell location
            handleCellInput(input);
        } else {
            printCommandErrorMessage();
        }
    }
}

// handles single letter commands
void handleCommand(char command, bool *stop) {
    if (command == 'e') {
        //exit
        printEndMessage();
        *stop = true;
    } else if (command == 'r') {
        //clear board
        resetGrid();

        printPanel();
    } else if (command == 'g') {
        //generate valid board
        resetGrid();
        genGrid();

        printPanel();
    } else if (command == 'h') {
        //check inHelp toggle
        if (inHelp) {
            //close help page
            printPanel();
        } else {
            //open help page
            printHelpMessage();
        }
    } else if (command == 's') {
        //attempt to solve board
        bool solved = solveGrid();

        printPanel();

        //if not solved, print error message
        if (!solved) {
            printUnsolvableMessage();
        }
    } else if (command != '\n') {
        printCommandErrorMessage();
    }
}

// handles cell location and number inputs
void handleCellInput(char *cell) {
    char rowChar;
    int col, num;

    //extract row character and column number from input
    sscanf(cell, "%c%d", &rowChar, &col);

    //set row and column to 0-indexed int
    int row = rowChar - 'A';
    col--;

    //get the number (to put in the cell)
    scanf("%d", &num);

    //make sure the cell location and number are within range
    if (row < 0 || row > 8 || col < 0 || col > 8 || num < EMPTY || num > 9) {
        printCommandErrorMessage();
    } else if (updateGrid(row, col, num, true)) {
        //if cell was updated, print the updated grid
        printPanel();
    }
}

// clears the board
void resetGrid() {
    //set unique to false since the board is empty
    unique = false;

    //update grid and given vars
    numGivens = 0;
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            grid[row][col] = EMPTY;
            given[row][col] = false;
        }
    }
}

// solves board if the board is unique
bool solveGrid() {
    if (unique) {
        //fills board with solution
        genSolution(0, 0);

        return true;
    }
    return false;
}

// generates a valid board
void genGrid() {
    //TBD - hardcoded for now
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
    numGivens = 28;
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
    unique = true;
}

// updates a cell in the board
bool updateGrid(int row, int col, int num, bool isGiven) {
    //if updating the board won't break the puzzle
    if (isValidDeep(row, col, num)) {
        //update the number of givens
        if (num == EMPTY) {
            //delete a given
            numGivens -= 1;
        } else if (grid[row][col] == EMPTY) {
            //filled an empty cell
            numGivens += 1;
        } //if filled a non empty cell, number of givens doesn't change

        //update grid and given
        grid[row][col] = num;
        given[row][col] = isGiven;

        //return that the board was updated
        return true;
    } else {
        //if puzzle will break, don't update and print an error message
        printInvalidInputMessage(row, col, num);
        return false;
    }
}