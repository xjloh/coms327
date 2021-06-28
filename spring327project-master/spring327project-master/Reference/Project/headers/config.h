#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>
#include <stdio.h>

#include "moves.h"


struct boardConfig {
    char turn[6];
    int redPawns;
    int redKings;
    int blackPawns;
    int blackKings;
    char board[8][8];
};
typedef struct boardConfig BoardConfig;

/**
 * configuration structure that holds all the data collected
 * from analyzing the config file, minus the moves made
 */
struct config {
    BoardConfig boardConfig;
    char jumpRule[15];
    char captureRule[11];
    bool isFlipped;
    Move *moves;
    int invalidMoveIndex;
    int numMovesProcessed;
    int numMoves;
    char* fileName;
    bool showMessages;
};
typedef struct config Config;

extern Config setup;

/**
 * Runs logic to ensure passed in config through stdin is formatted correctly
 */
void evaluateConfigFile(FILE *file);

/**
 * Evaluates if the given character should be skipped or not
 *
 * @param c character to evaluate
 * @param currentLine line number to update
 * @param skipSpaces bool, true to skip spaces
 * @param skipNewLine bool, true to skip new lines
 * @return bool on weather to skip character
 */
bool skipCharacter(char c, int *currentLine, FILE *file, bool skipSpaces, bool skipNewLine);

/**
 * Checks that the rule's section is formatted correctly
 * @param c character to start evaluating at
 * @param currentLine line number for handling errors
 */
void validateRule(char c, FILE *file, int *currentLine);

/**
 * Checks that the turn section is formatted correctly
 * @param c character to start evaluating at
 * @param currentLine line number for handling errors
 */
void validateTurn(char c, FILE *file, int *currentLine);

/**
 * Checks that the board section is formatted correctly
 * @param c character to start evaluating at
 * @param currentLine line number for handling errors
 */
void validateBoard(char c, FILE *file, int *currentLine);

/**
 * Checks that the move section is formatted correctly
 * @param c character to start evaluating at
 * @param currentLine line number for handling errors
 */
void validateMoves(char c, FILE *file, int *currentLine);

/**
 * Prints the information required for a valid configuration
 */
void printValidConfigInfo();

#endif
