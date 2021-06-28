#ifndef MOVES_H
#define MOVES_H

#include <stdio.h>
#include <stdbool.h>

struct position {
    int row;
    int col;
};
typedef struct position Position;

struct files {
    FILE **files;
    int numFiles;
};
typedef struct files Files;

struct moveFlags {
    Files eFiles;
    Files hFiles;
    int movesToExecute;
};
typedef struct moveFlags MoveFlags;
extern MoveFlags mFlags;

struct move {
    Position *positions;
    int numPositions;
};
typedef struct move Move;

/**
 * Handles logic around aggregating over stored positions
 * within moves and moves from config file.
 */
bool executeMoves(int index, bool ignoreFlags, bool updateInvalidMove);


/**
 * Calls executeMoves and other functions to print info
 * to other files / stdout
 */
void exportFiles();

/**
 * Determine if the move is possible for the given locations
 * based on current turn and rules
 * @param startRow row loc of piece to move
 * @param startCol col loc of piece to move
 * @param endRow row loc of where piece is trying to move
 * @param endCol end loc of where piece is trying to move
 * @param isRed bool if piece is red or black
 * @return bool if move is possible
 */
bool validPositionToMove(
        int startRow, int startCol, int endRow, int endCol,
        bool isRed, Move *captureMoves, int numPositions);

/**
 * Based on the current piece, determine if it can jump a piece
 * @param isBlackKing bool if current piece is a black king
 * @param isRedKing bool if current piece is a red king
 * @param jumpedPiece piece to be jumped
 * @return bool if is valid piece to jump
 */
bool jumpPiece(bool isBlackKing, bool isRedKing, char jumpedPiece, bool jump, bool isRed);


/**
 * Formats and prints a move to a file
 * @param move to print
 * @param file to print to
 */
void exportMove(Move move, FILE *file);

/**
 * Prints all unprocessed moves to a file
 * @param file to print to
 */
void exportUnprocessedMoves(FILE *file, bool isValid);

/**
 * Prints to stdout either success or error on move validation
 * @param isValid bool if there is an illegal move
 */
void printMoveValidation(bool isValid);

/**
 * Prints game board formatted for the exchange flag
 */
void exportExchangeBoard(FILE *f);

/**
 * Prints contents for the exchange flag
 */
void exportExchangeFile(bool isValid);

/**
 * Prints contents for the human flag
 */
void exportHumanFile(bool isValid);

/**
 * Prints game board formatted for the human flag
 */
void exportHumanBoard(FILE *f);


/**
 * Evaluates if there are any possible jump moves based on piece position
 * @param startRow start piece row
 * @param startCol start piece col
 * @param isRed if red's turn
 * @return list of possible jump moves (combo of L & R)
 */
void findCapturePositions(Move* capturePositions, int startRow, int startCol, bool isRed, bool jump);

/**
 * Format all moves with 4 spaces between pairs followed by new line
 * @param file to print to
 */
void exportAllMoves(FILE *file);

/**
 * Determine if a position to capture is valid
 * @param row of potential position
 * @param col of potential position
 * @return true if valid, else false
 */
bool validCaptureMove(int row, int col);

/**
 * Ensure row col jumping to is empty and within bounds
 * @param row of end position
 * @param col of end position
 * @return true if valid else false
 */
bool validCaptureMove(int row, int col);

#endif