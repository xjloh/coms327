#include "headers/helpers.h"
#include "headers/config.h"
#include "headers/moves.h"
#include "headers/rank.h"

MoveFlags mFlags;
RankScore rankScore;

int mapLetterToInt(char letter) {
#ifdef DEBUG
    printf("To Convert: %c\n", letter);
#endif
    switch (letter) {
        case 'a':
            return 0;
        case 'b':
            return 1;
        case 'c':
            return 2;
        case 'd':
            return 3;
        case 'e':
            return 4;
        case 'f':
            return 5;
        case 'g':
            return 6;
        case 'h':
            return 7;
        default:
            return -1;
    }
}

char mapIntToLetter(int num) {
    switch (num) {
        case 0:
            return 'a';
        case 1:
            return 'b';
        case 2:
            return 'c';
        case 3:
            return 'd';
        case 4:
            return 'e';
        case 5:
            return 'f';
        case 6:
            return 'g';
        case 7:
            return 'h';
        default:
            return 0;
    }
}

void deallocateChangeInfo() {
    unsigned long m;
    for (m = 0; m < setup.numMoves; m++) {
        free(setup.moves[m].positions);
    }
    free(setup.moves);

    free(mFlags.hFiles.files);
    free(mFlags.eFiles.files);

}

void deallocateRankMoves() {
    unsigned long m;
    for (m = 0; m < rankScore.numPossibleMoves; m++) {
        free(rankScore.possibleMoves[m].move.positions);
    }
    free(rankScore.possibleMoves);
}

char *pieceToTurn(char piece) {
    switch (piece) {
        case 'r':
        case 'R':
            return "red";
        case 'b':
        case 'B':
            return "black";
        default:
            return 0;
    }
}

bool isRed(char *turn) {
    if (strcmp(turn, "red") == 0) {
        return true;
    }
    return false;
}

int rowToBoardRow(int row) {
    switch (row) {
        case 0:
            return 8;
        case 1:
            return 7;
        case 2:
            return 6;
        case 3:
            return 5;
        case 4:
            return 4;
        case 5:
            return 3;
        case 6:
            return 2;
        case 7:
            return 1;
        default:
            return -1;
    }
}

int boardRowToRow(int boardRow) {
    switch (boardRow) {
        case 8:
            return 0;
        case 7:
            return 1;
        case 6:
            return 2;
        case 5:
            return 3;
        case 4:
            return 4;
        case 3:
            return 5;
        case 2:
            return 6;
        case 1:
            return 7;
        default:
            return -1;
    }
}

void copyBoardConfig(BoardConfig* src, BoardConfig* dest){
    unsigned int r;
    unsigned int c;
    for (r = 0; r < 8; r++) {
        for (c = 0; c < 8; c++) {
            dest->board[r][c] = src->board[r][c];
        }
    }
    dest->blackPawns = src->blackPawns;
    dest->blackKings = src->blackKings;
    dest->redPawns = src->redPawns;
    dest->redKings = src->redKings;
    strncpy(dest->turn, src->turn, 6);
}


void changeTurnRed(char *setTurn) {
    size_t destination_size = sizeof(setTurn);
    strncpy(setTurn, "red", destination_size);
    setTurn[destination_size - 1] = '\0';
}

void changeTurnBlack(char *setTurn) {
    size_t destination_size = sizeof(setTurn);
    strncpy(setTurn, "black", destination_size);
    setTurn[destination_size - 1] = '\0';
}

void alternateTurn(char *setTurn) {
    if (strcmp(setTurn, "red") == 0) {
        changeTurnBlack(setTurn);
    } else {
        changeTurnRed(setTurn);
    }
}

bool inIntList(int* list, int size, int value){
    unsigned int i;
    int current;
    for(i = 0; i < size; i++){
        current = list[i];
        if(value == current){
            return true;
        }
    }
    return false;
}