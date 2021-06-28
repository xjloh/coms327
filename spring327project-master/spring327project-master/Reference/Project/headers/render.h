#ifndef RENDER_H
#define RENDER_H

#include <stdbool.h>
#include "moves.h"

/**
 * Renders checkerboard with piece positions
 */
void renderBoard();

/**
 * Renders hotkey instructions and turn headers
 */
void renderInstructions();

/**
 * Highlights/un-highlights move at specific position with color code
 * @param cx cursor x position
 * @param cy cursor y position
 * @param moveText text to highlight
 * @param type of move to color code
 * @param reset to default value
 */
void highlightMove(int cx, int cy, char* moveText, char type, bool reset);

/**
 * Returns if move is invalid (i), valid (v) or unknown (u) or empty (e)
 * @param index of move to evualate type
 * @return char of type (i, v, u, e)
 */
char getMoveType(int index);

/**
 * Formats a move into cr->cr...
 * @param move to format if any
 * @param index of move -1 if not within bounds
 * @return string of formatted move
 */
char* getMoveText(Move move, int index);

/**
 * Shows the file name and move index out of total moves
 * @param index of current move
 */
void renderMoveLocationFileName(int index);

/**
 * Render foramtted moves in range start to end of all moves
 * @param start index
 * @param end index
 */
void renderMoves(int start, int end);

/**
 * Clears the last line of the terminal (edit line)
 */
void clearEditLine();

/**
 * Show on the edit line the move to save and the editable position to add to move
 * @param move to save
 * @param newPos that is being edited
 * @param editIndex 0/1 col or row
 */
void showEditMove(Move move, Position newPos, int editIndex);

/**
 * Clean slate to re render move list
 */
void clearMoveLines();

/**
 * Opens file and writes over contents with updates (append/edit moves)
 */
void updateInputFile();

#endif
