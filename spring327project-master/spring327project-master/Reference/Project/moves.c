#include "headers/config.h"
#include "headers/helpers.h"
#include "headers/moves.h"

//#define DEBUG

Config setup;
MoveFlags mFlags;

void exportFiles() {
    bool isValid = executeMoves(setup.numMoves, false, false);

    if (mFlags.eFiles.files) {
        exportExchangeFile(isValid);
    }
    if (mFlags.hFiles.files) {
        exportHumanFile(isValid);
    }

    printMoveValidation(isValid);
}

bool executeMoves(int index, bool ignoreFlags, bool updateInvalidMove) {
    BoardConfig copy;
    unsigned long m;
    unsigned long p = 0;
    char startPiece;
    bool isRedTurn = false;
    if (strcmp(setup.boardConfig.turn, "red") == 0) {
        isRedTurn = true;
    }
    char *currentTurn;
    Position origin;
    Position dest;

    Move *captureMoves = malloc(sizeof(Move));
    if(!captureMoves){
        fprintf(stderr, "Malloc failed on captureMoves.\n");
        exit(1);
    }
    bool validMove;

    for (m = 0; m < index; m++) {
        // early break if no moves should be evaluated
        if (!ignoreFlags && mFlags.movesToExecute == 0) {
            break;
        }
        copyBoardConfig(&setup.boardConfig, &copy);
        setup.numMovesProcessed++;
#ifdef DEBUG
        printf("Processed: %d\n", setup.numMovesProcessed);
#endif
        Move move = setup.moves[m];
        p = 0;
        while (p < move.numPositions - 1) {
            // single jumps should only pass through while once
            if (strcmp(setup.jumpRule, "single jumps") == 0 && move.numPositions > 2) {
                if(setup.showMessages){
                    fprintf(stderr, "Multiple jumps is not allowed.\n");
                }
                if(updateInvalidMove) {
                    setup.invalidMoveIndex = m;
                }
                return false;
            }
            // have to convert row to board row since 0/1 is at bottom not top
            origin.row = move.positions[p].row;
            origin.col = move.positions[p].col;

            p++;
            dest.row = move.positions[p].row;
            dest.col = move.positions[p].col;

            startPiece = setup.boardConfig.board[origin.row][origin.col];
            currentTurn = pieceToTurn(startPiece);
            /*
             * Illegal for the following reasons:
             * - not a piece (ie. not read or black)
             * - is red's turn but startPiece is black
             * - is black's turn but startPiece is red
             */
            if (!currentTurn ||
                (strcmp(currentTurn, "red") == 0 && !isRedTurn) ||
                (strcmp(currentTurn, "black") == 0 && isRedTurn)) {
#ifdef DEBUG
                if(isRedTurn) {
                    if(setup.showMessages){
                        fprintf(stderr, "Turn Error: should be `r` or `R` but was given '%c'\n", startPiece);
                    }
                }
                else {
                    if(setup.showMessages){
                        fprintf(stderr, "Turn Error: should be `b` or `B` but was given '%c'\n", startPiece);
                    }
                }
                if(setup.showMessages){
                    fprintf(stderr, "Position: row(%d) col(%d)\n", startRow, startCol);
                }
#endif
                if(updateInvalidMove) {
                    setup.invalidMoveIndex = m;
                }
                return false;
            }

            if (strcmp(setup.captureRule, "capture") == 0) {
                findCapturePositions(captureMoves, origin.row, origin.col, isRedTurn, true);
            }
            validMove = validPositionToMove(
                    origin.row, origin.col, dest.row, dest.col,
                    isRedTurn, captureMoves, move.numPositions);
            if (!validMove) {
#ifdef DEBUG
                fprintf(stderr, "Invalid position to move to:\n"
                                "From col(%d) row(%d) \n"
                                "To col(%d) row(%d)\n", startCol, startRow, endCol, endRow);
#endif
                if (strcmp(setup.jumpRule, "multiple jumps") == 0) {
#ifdef DEBUG
                    fprintf(stderr, "Reverting board.\n");
#endif
                    copyBoardConfig(&copy, &setup.boardConfig);
                }
                if(updateInvalidMove) {
                    setup.invalidMoveIndex = m;
                }
                return false;
            }
            // update start position with blank black space
            setup.boardConfig.board[origin.row][origin.col] = '.';

            // pawn to king
            if (isRedTurn && dest.row == 0 && startPiece == 'r') {
                setup.boardConfig.redKings++;
                setup.boardConfig.redPawns--;
                startPiece = 'R';
                if (p != move.numPositions - 1) {
                    if(setup.showMessages) {
                        fprintf(stderr, "Invalid move. Pawn to King end of turn.\n");
                    }
                    if(updateInvalidMove) {
                        setup.invalidMoveIndex = m;
                    }
                    return false;
                }
            }
            if (!isRedTurn && dest.row == 7 && startPiece == 'b') {
                setup.boardConfig.blackKings++;
                setup.boardConfig.blackPawns--;
                startPiece = 'B';
                if (p != move.numPositions - 1) {
                    if(setup.showMessages) {
                        fprintf(stderr, "Invalid move. Pawn to King end of turn.\n");
                    }
                    if(updateInvalidMove) {
                        setup.invalidMoveIndex = m;
                    }
                    return false;
                }
            }
#ifdef DEBUG
            printf("BK: %d\n", setup.blackKings);
            printf("RK: %d\n", setup.redKings);
            printf("BP: %d\n", setup.blackPawns);
            printf("RP: %d\n", setup.redPawns);
#endif

            // set new move
            setup.boardConfig.board[dest.row][dest.col] = startPiece;

            // switch turns
            // if all positions of move have been evaluated
            if (p == (move.numPositions - 1)) {
                if (isRedTurn) {
                    isRedTurn = false;
                    changeTurnBlack(setup.boardConfig.turn);
                } else {
                    isRedTurn = true;
                    changeTurnRed(setup.boardConfig.turn);
                }
            }
#ifdef DEBUG
            printf("isRead: %d\n", isRedTurn);
            printf("Moves: %d\n", move.numPositions);
            printf("P: %lu\n", p);
            printf("Turn: %s\n", setup.turn);
#endif
        }
        // enforce max number of moves
        if (!ignoreFlags && setup.numMovesProcessed == mFlags.movesToExecute) {
            break;
        }
    }
    free(captureMoves);
    if(updateInvalidMove){
        setup.invalidMoveIndex = -1;
    }
    return true;
}

void findCapturePositions(Move* capturePositions, int startRow, int startCol, bool isRed, bool jump) {
    capturePositions->positions = malloc(sizeof(Position));
    if (!capturePositions->positions) {
        fprintf(stderr, "Malloc error on `capturePositions->positions`\n");
        exit(1);
    }

    capturePositions->numPositions = 0;

    char startPiece = setup.boardConfig.board[startRow][startCol];
    Position dest;

    bool isRedKing = false;
    bool isBlackKing = false;

    // determine if piece is a king
    if (startPiece == 'B') {
        isBlackKing = true;
    }
    if (startPiece == 'R') {
        isRedKing = true;
    }

    char jumpedPiece;

    if (isRed || isBlackKing) {
        // right
        jumpedPiece = setup.boardConfig.board[startRow - 1][startCol + 1];
        if (validCaptureMove(startRow-2, startCol+2) && jumpPiece(isBlackKing, isRedKing, jumpedPiece, jump, isRed)) {
            dest.row = startRow - 1;
            dest.col = startCol + 1;
            capturePositions->positions[capturePositions->numPositions++] = dest;
            capturePositions->positions = realloc(capturePositions->positions, (capturePositions->numPositions + 1) * sizeof(char));
            if (!capturePositions->positions) {
                fprintf(stderr, "Realloc error on captureMoves.\n");
                exit(1);
            }
        }
        // left
        jumpedPiece = setup.boardConfig.board[startRow - 1][startCol - 1];
        if (validCaptureMove(startRow-2, startCol-2) && jumpPiece(isBlackKing, isRedKing, jumpedPiece, jump, isRed)) {
            dest.row = startRow - 1;
            dest.col = startCol - 1;
            capturePositions->positions[capturePositions->numPositions++] = dest;
            capturePositions->positions = realloc(capturePositions->positions, (capturePositions->numPositions + 1) * sizeof(char));
            if (!capturePositions->positions) {
                fprintf(stderr, "Realloc error on captureMoves.\n");
                exit(1);
            }
        }

    }
    if (!isRed || isRedKing) {
        // right
        jumpedPiece = setup.boardConfig.board[startRow + 1][startCol + 1];
        if (validCaptureMove(startRow+2, startCol+2) &&  jumpPiece(isBlackKing, isRedKing, jumpedPiece, jump, isRed)) {
            dest.row = startRow + 1;
            dest.col = startCol + 1;
            capturePositions->positions[capturePositions->numPositions++] = dest;
            capturePositions->positions = realloc(capturePositions->positions, (capturePositions->numPositions + 1) * sizeof(char));
            if (!capturePositions->positions) {
                fprintf(stderr, "Realloc error on captureMoves.\n");
                exit(1);
            }
        }
        // left
        jumpedPiece = setup.boardConfig.board[startRow + 1][startCol - 1];
        if (validCaptureMove(startRow+2, startCol-2) && jumpPiece(isBlackKing, isRedKing, jumpedPiece, jump, isRed)) {
            dest.row = startRow + 1;
            dest.col = startCol - 1;
            capturePositions->positions[capturePositions->numPositions++] = dest;
            capturePositions->positions = realloc(capturePositions->positions, (capturePositions->numPositions + 1) * sizeof(char));
            if (!capturePositions->positions) {
                fprintf(stderr, "Realloc error on captureMoves.\n");
                exit(1);
            }
        }
    }
}

bool validCaptureMove(int row, int col){
    if(row < 0 || col < 0 || row > 7 || col > 7){
        return false;
    }
    char piece = setup.boardConfig.board[row][col];
    if (piece != '.'){
        return false;
    }
    return true;
}

bool validPositionToMove(
        int startRow, int startCol, int endRow, int endCol,
        bool isRed, Move *captureMoves, int numPositions
) {
    char endPiece = setup.boardConfig.board[endRow][endCol];
    char startPiece = setup.boardConfig.board[startRow][startCol];

    bool isRedKing = false;
    bool isBlackKing = false;

    if (startPiece == 'B') {
        isBlackKing = true;
    }
    if (startPiece == 'R') {
        isRedKing = true;
    }

    char jumpedPiece;


    if (endPiece != '.') {
#ifdef DEBUG
        fprintf(stderr, "Can't move to red space\n");
#endif
        return false;
    }

    // red moves up
    if (isRed || isBlackKing) {
        // single move
        if (endRow == (startRow - 1)) {
            if (numPositions > 2) {
                if(setup.showMessages) {
                    fprintf(stderr, "Can't make single move in multi jump move\n");
                }
                return false;
            }
#ifdef DEBUG
            unsigned long i;
            for(i = 0; i < strlen(captureMoves); i++){
                printf("%c ", captureMoves[i]);
            }
            printf("\n");
#endif
            if (captureMoves && captureMoves->numPositions > 0) {
                if(setup.showMessages) {
                    fprintf(stderr, "Invalid move. Must capture opponent.\n");
                }
                return false;
            }
            if (endCol != (startCol + 1) && endCol != (startCol - 1)) {
                if (!isBlackKing) {
                    return false;
                }
            }
            return true;
        }
            // jump
            // red = row - 1 (board grid coordinates, moving up on board)
        else if (endRow == (startRow - 2)) {
            // right
            // col + 1
            if (endCol == (startCol + 2)) {
                jumpedPiece = setup.boardConfig.board[startRow - 1][startCol + 1];
                if (!jumpPiece(isBlackKing, isRedKing, jumpedPiece, true, isRed)) {
                    return false;
                }
                setup.boardConfig.board[startRow - 1][startCol + 1] = '.';
                return true;
            }
                // left
                // col - 1
            else if (endCol == (startCol - 2)) {
                jumpedPiece = setup.boardConfig.board[startRow - 1][startCol - 1];
                if (!jumpPiece(isBlackKing, isRedKing, jumpedPiece, true, isRed)) {
                    return false;
                }
                setup.boardConfig.board[startRow - 1][startCol - 1] = '.';
                return true;
            }
        } else if (!isRedKing && !isBlackKing) {
            return false;
        }
    }
    if (!isRed || isRedKing) {
        // single move
        if (endRow == (startRow + 1)) {
            if (numPositions > 2) {
                if(setup.showMessages) {
                    fprintf(stderr, "Can't make single move in multi jump move\n");
                }
                return false;
            }
            if (captureMoves && captureMoves->numPositions > 0) {
                if(setup.showMessages) {
                    fprintf(stderr, "Invalid move. Must capture opponent.\n");
                }
                return false;
            }
            if (endCol != (startCol + 1) && endCol != (startCol - 1)) {
                if (!isRedKing) {
                    return false;
                }
            }
            return true;
        }
            // jump
            // black = row + 1 (board coordinates, moving down)
        else if (endRow == (startRow + 2)) {
            // right
            // col + 1
            if (endCol == (startCol + 2)) {
                jumpedPiece = setup.boardConfig.board[startRow + 1][startCol + 1];
                if (!jumpPiece(isBlackKing, isRedKing, jumpedPiece, true, isRed)) {
                    return false;
                }
                setup.boardConfig.board[startRow + 1][startCol + 1] = '.';
                return true;
            }
                // left
                // col - 1
            else if (endCol == (startCol - 2)) {
                jumpedPiece = setup.boardConfig.board[startRow + 1][startCol - 1];
                if (!jumpPiece(isBlackKing, isRedKing, jumpedPiece, true, isRed)) {
                    return false;
                }
                setup.boardConfig.board[startRow + 1][startCol - 1] = '.';
                return true;
            }
        } else {
            return false;
        }
    }
    return true;
}

bool jumpPiece(bool isBlackKing, bool isRedKing, char jumpedPiece, bool jump, bool isRed) {
    // jumped piece must be read or black piece
    if (jumpedPiece == '.' || jumpedPiece == '"') {
        return false;
    }

    // ensure not jumping own color
    if (jumpedPiece == 'r' || jumpedPiece == 'R') {
        if (isRedKing || isRed) {
            return false;
        }
    }
    if (jumpedPiece == 'b' || jumpedPiece == 'B') {
        if (isBlackKing || !isRed) {
            return false;
        }
    }
    if(!jump) {
        return true;
    }
    // update setup piece count based on jumped piece
    switch (jumpedPiece) {
        case 'b':
            setup.boardConfig.blackPawns--;
            break;
        case 'B':
            setup.boardConfig.blackKings--;
            break;
        case 'r':
            setup.boardConfig.redPawns--;
            break;
        case 'R':
            setup.boardConfig.redKings--;
            break;
#ifdef DEBUG
            fprintf(stderr, "BUG: Jumped piece not valid");
#endif
    }
    return true;
}

void printMoveValidation(bool isValid) {
    fprintf(stdout, "Input file has %d moves\n"
                    "Processing %d moves\n", setup.numMoves, setup.numMovesProcessed);
    if (isValid) {
        fprintf(stdout, "All moves are legal\n");
    } else {
        fprintf(stdout, "Move %d is illegal: ", setup.numMovesProcessed);
        exportMove(setup.moves[setup.numMovesProcessed - 1], stdout);
        fprintf(stdout, "\n");
    }
}

void exportMove(Move move, FILE *file) {
    unsigned i;
    // map to valid board move
    for (i = 0; i < move.numPositions; i++) {
        char col = mapIntToLetter(move.positions[i].col);
        int row = rowToBoardRow(move.positions[i].row);
        if (i == 0) {
            fprintf(file, "%c%d", col, row);
        } else {
            fprintf(file, "->%c%d", col, row);
        }
    }
}

void exportExchangeBoard(FILE *f) {
    unsigned long r;
    unsigned long c;

    int boardRow = 0;
    int boardCol = 0;

    for (r = 0; r < 10; r++) {
        for (c = 0; c < 17; c++) {
            if (r == 0 || r == 9) {
                fprintf(f, "-----------------");
                break;
            }
            if (c % 2 == 0) {
                fprintf(f, "|");
            } else {
                fprintf(f, "%c", setup.boardConfig.board[boardRow][boardCol]);
                boardCol++;
                if (boardCol == 8) {
                    boardRow++;
                    boardCol = 0;
                }
            }
        }
        fprintf(f, "\n");
    }
}

void exportExchangeFile(bool isValid) {
    unsigned i;
    for (i = 0; i < mFlags.eFiles.numFiles; i++) {
        FILE *f = mFlags.eFiles.files[i];
        fprintf(f, "RULES:\n\t%s\n\t%s\n", setup.captureRule, setup.jumpRule);
        fprintf(f, "TURN:\n\t%s\n", setup.boardConfig.turn);

        fprintf(f, "BOARD:\n");
        exportExchangeBoard(f);

        fprintf(f, "MOVES:\n");
        exportUnprocessedMoves(f, isValid);
        fclose(f);
    }
}

void exportUnprocessedMoves(FILE *file, bool isValid) {
    unsigned long i;
    // depending on if moves are valid or not, determines inclusive or exclusive
    // for the number of moves actually processed
    if (!isValid) {
        for (i = setup.numMovesProcessed - 1; i < setup.numMoves; i++) {
            exportMove(setup.moves[i], file);
            fprintf(file, "\n");
        }
    } else {
        for (i = setup.numMovesProcessed; i < setup.numMoves; i++) {
            exportMove(setup.moves[i], file);
            fprintf(file, "\n");
        }
    }
}

void exportAllMoves(FILE *file){
    unsigned int i;
    bool addSpace = true;
    bool addNewLine = false;
    for(i = 0; i < setup.numMoves-1; i++){
        exportMove(setup.moves[i], file);

        if(addSpace) {
            fprintf(file, "    ");
            addSpace = false;
            addNewLine = true;
        }
        else if(addNewLine) {
            fprintf(file, "\n");
            addSpace = true;
            addNewLine = false;
        }
    }
}

void exportHumanFile(bool isValid) {
    unsigned int i;
    for (i = 0; i < mFlags.hFiles.numFiles; i++) {
        FILE *f = mFlags.hFiles.files[i];
        exportHumanBoard(f);
        exportUnprocessedMoves(f, isValid);
        fclose(f);
    }

}

void exportHumanBoard(FILE *f) {
    unsigned long r;
    unsigned long c;

    char alpha[] = "abcdefgh";
    char nums[] = "8877665544332211";
#ifdef DEBUG
    char rowNums[] = "0011223344556677";
    char colNums[] = "    0   1   2   3   4   5   6   7  ";
#endif
    char divider[] = "  +---+---+---+---+---+---+---+---+";
    int alphaLoc = 0;
    int numsLoc = 0;

    int boardRow = 0;
    int boardCol = 0;

    bool dividerNext = true;

#ifdef DEBUG
    fprintf(f, "%s\n", colNums);
#endif

    for (r = 0; r < 19; r++) {
        for (c = 0; c < 37; c++) {
            if ((c % 4 == 0) && (r == 0 || r == 18)) {
                if (c == 0 || c == 36) {
                    fprintf(f, "%c", ' ');
                } else {
                    // build alpha lines
                    fprintf(f, "%*c", 4, alpha[alphaLoc]);
                    alphaLoc++;
                }
            } else if ((r != 0 && r != 18 && r % 2 == 0) && (c == 0 || c == 36)) {
                // build number rows
#ifdef DEBUG
                if (c == 36) {
                    fprintf(f, "%c ", rowNums[numsLoc]);
                }
#endif
                fprintf(f, "%c", nums[numsLoc]);
                numsLoc++;
            } else if ((r % 2 != 0)) {
                // print divider lines
                fprintf(f, "%s", divider);
                break;
            } else if (r != 0 && r != 18) {
                if (c == 1 || c == 35 || (c % 2 != 0)) {
                    fprintf(f, "%s", " ");
                } else {
                    if (dividerNext) {
                        fprintf(f, "|");
                        dividerNext = false;
                    } else {
                        fprintf(f, "%c", setup.boardConfig.board[boardRow][boardCol]);
                        boardCol++;
                        if (boardCol == 8) {
                            boardRow++;
                            boardCol = 0;
                        }
                        dividerNext = true;
                    }
                }
            }
        }
        fprintf(f, "\n");
        alphaLoc = 0;
        dividerNext = true;
    }
    fprintf(f, "\n");
}
