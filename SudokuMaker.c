#include <stdio.h>

void printBold(char *str) {
    printf("\e[1m%s\e[0m", str);
}

void printGray(char *str) {
    printf("\e[90m%s\e[0m", str);
}

void printGrid(int *grid) {
    for (int row = 0; row < 9; ++row) {
        if (row % 3 == 0) {
            printBold("+-----------+-----------+-----------+\n|");
        } else {
            printBold("|");
            for (int i = 0; i < 3; ++i) {
                printGray("---+---+---");
                printBold("|");
            }
            printBold("\n|");
        }
        for (int col = 0; col < 9; ++col) {
            int num = grid[row * 9 + col];
            if (num == -1) {
                printf("   ");
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
    printBold("+-----------+-----------+-----------+\n");
}

int main(void) {
    int grid[81];
    for (int i = 0; i < 81; ++i) {
        grid[i] = (i + i / 9) % 9 + 1;
    }

    printGrid(grid);

    return 0;
}