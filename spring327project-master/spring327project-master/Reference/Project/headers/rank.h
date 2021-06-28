#ifndef RANK_H
#define RANK_H

#include <stdbool.h>
#include "moves.h"
#include "config.h"

typedef struct pm PossibleMoves;
struct pm {
    Move move;
    BoardConfig config;
    int score;
    int depthLevel;
    bool isJump;
};

typedef struct rs RankScore;
struct rs {
    PossibleMoves *possibleMoves;
    bool isVerbose;
    int depth;
    int numPossibleMoves;
    PossibleMoves *captureMoves;
    int numCaptureMoves;
};
extern RankScore rankScore;

/**
 * Runner for Part3, calls: findOriginMove, setDeepestScores, setNonDeepestScores, printPossibleMoves
 */
void calculateRank();

/**
 * Recursive function to find possible moves
 * @param depth current depth looking at moves for
 */
void findOriginMove(int depth);

/**
 * 99 = winning config
* redScore = -blackScore
 * Uses the stored config of the move to calculate score
 *
 * @param possibleMove move to calculate score for
 * @return int score for move
 */
int calculateScore(PossibleMoves *possibleMove);

/**
 * Helper function for recursive function
 * @param origin position to check possible moves for
 * @param piece current piece at that position
 * @param depth current depth of the move, to be passed back into recursive function incremented
 */
void findPossibleMoves(Position origin, char piece, int depth);

/**
 * Adds move to list of possible moves for the game
 * @param origin start position
 * @param dest end position
 * @param depth current depth of that move
 */
void addPossibleMove(Position origin, Position dest, int depth, bool isJump);

/**
 * If a move is valid, set dest with origin piece and reset origin position
 * also manage if move creates a King
 * @param origin start position
 * @param dest end position
 * @param piece from start position
 * @param isRedTurn if is red's turn
 */
void handleValidMove(Position origin, Position dest, char piece, bool isRedTurn);

/**
 * Prints output formatting for all collected possible positions for depth d
 */
void printPossibleMoves();

/**
 * Given a list of scores, return the max score
 * @param scores list of scores
 * @param numScores number of scores in list
 * @return max score from list
 */
int maxScore(int *scores, int numScores);

/**
 * Prints formatting of moves that start with a `.`
 * @param possibleMove move to print
 */
void printPeriodMove(PossibleMoves possibleMove);

/**
 * Prints formatting of moves that start with a `?`
 * @param possibleMove move to print
 */
void printQuestionMarkMove(PossibleMoves possibleMove);

/**
 * Given a list of moves that haven't been printed, print those moves with periods
 * (ie. those printed with `?` with some exceptions)
 * @param notPrinted list of index of not printed moves
 * @param numNotPrinted number of moves that haven't been printed
 */
void printNotPrintedList(int *notPrinted, int numNotPrinted);

/**
 * Calculates scores of all the deepest level moves while
 * setting all other scores = 100
 */
void setDeepestScores();

/**
 * Look for scores = 100, calculate score of moves with depth < maxDepth
 * starting with those moves with depth = maxDepth - 1
 */
void setNonDeepestScores();


/**
 * Alphabetizes position by col and then row (ie. letter than number)
 * @param piecePositions list of positions
 * @param numPositions number of positions in list
 */
void alphabetize(Position* piecePositions, int numPositions);

/**
 * If a depth has a single jump option, don't include any
 * non jump moves at that depth
 * used for capture extra credit
 */
void filterCaptureMoves();
#endif