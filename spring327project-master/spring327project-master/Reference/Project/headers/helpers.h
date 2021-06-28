#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "config.h"

/**
 * Maps a letter a-f into its corresponding
 * int value starting at 0
 *
 * @param letter to map to an int
 * @return the int value of the letter or -1 if out of bounds
 */
int mapLetterToInt(char letter);

/**
 * Maps an int 0-7 into its corresponding
 * str value starting at a
 *
 * @param num to map to letter
 * @return the char of the int
 */
char mapIntToLetter(int num);

/**
 * Frees all memory that was malloced in executing info.c & change.c
 */
void deallocateChangeInfo();

/**
 * Determine piece color
 * @param piece in [r , R, b, B]
 * @return string of turn in [red, black]
 */
char *pieceToTurn(char piece);

/**
 * Converts row to board row since board rows
 * go from 8 to 1 instead of 0 to 7
 * @param row stored based on array location
 * @return board row
 */
int rowToBoardRow(int row);

/**
 * Converts board rows 8 to 1 to rows stored based on array location  0 - 7
 * @param boardRow to convert to array location
 * @return row in array
 */
int boardRowToRow(int boardRow);

/**
 * copy's the src BoardConfig over to the dest BoardConfig
 */
void copyBoardConfig(BoardConfig *dest, BoardConfig *src);

/**
 * switches setTurn to red
 */
void changeTurnRed(char *setTurn);

/**
 * switches setTurn to black
 */
void changeTurnBlack(char *setTurn);

/**
 * Checks to see if a char array = 'red'
 * @param turn char array (red or black)
 * @return bool true if red, else false
 */
bool isRed(char *turn);

/**
 * Switches the passed in char array to opposite turn
 * (ie. if setTurn = red, will change setTurn = black
 * @param setTurn string turn (red or black)
 */
void alternateTurn(char *setTurn);

/**
 * Frees all memory allocated in executing rankmoves.c
 */
void deallocateRankMoves();

/**
 * Checks to see if a value is in a list of ints
 * @param list of ints
 * @param size of list
 * @param value looking for in list
 * @return bool if value in list
 */
bool inIntList(int* list, int size, int value);
#endif
