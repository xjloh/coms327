#include "headers/errors.h"

#include <stdio.h>
#include <stdlib.h>

void raiseFormattingError(int *lineNumber, char message[]) {
    fprintf(stderr, "Formatting error near line %d: %s\n", *lineNumber, message);
    exit(1);
}

void raiseBoardError(int *lineNumber, char correctChar, char foundChar, int row, int col) {
    if (correctChar == 'i') {
        fprintf(stderr,
                "Board error on line %d: '%c' on rank (row) %d file (col) %d is an invalid option.\n"
                "Character must be 'R', 'r', 'b', 'B', '.', or '\"'.\n",
                *lineNumber, foundChar, row, col);
    } else {
        fprintf(stderr,
                "Board error on line %d: expecting '%c' but received '%c' on rank (row) %d file (col) %d\n",
                *lineNumber, correctChar, foundChar, row, col);
    }
    exit(1);
}

void raiseMoveError(int *lineNumber, char correctChar, char foundChar) {
    switch (correctChar) {
        case 'l':
            fprintf(stderr, "Move error on line %d: expecting a letter between 'a-f' but received '%c'\n", *lineNumber,
                    foundChar);
            break;
        case 'n':
            fprintf(stderr, "Move error on line %d: expecting a letter between '1-8' but received '%c'\n", *lineNumber,
                    foundChar);
            break;
        case '-':
            fprintf(stderr, "Move error on line %d: expecting a '-' but received '%c'\n", *lineNumber, foundChar);
            break;
        case 'i':
            fprintf(stderr, "Move error on line %d: invalid position pair but received '%c'\n", *lineNumber, foundChar);
        default:
            fprintf(stderr, "Move error on line %d: expecting a '>' but received '%c'\n", *lineNumber, foundChar);
    }
    exit(1);
}