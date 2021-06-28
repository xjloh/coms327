#ifndef ERRORS_H
#define ERRORS_H

/**
 *  Formatting Error:
 * - expect keyword, instead other
 * - Unexpected or extra character
 * - unexpected end of file
 * @param lineNumber of error
 * @param message specific to formatting error
 */
void raiseFormattingError(int *lineNumber, char message[]);

/**
 * Board Error:
 * - piece where it shouldn't be
 * - empty black where there should be red
 * - DON"T CHECK num of pieces, just have to be oriented correctly
 * - error if first square not red
 * @param lineNumber of error
 * @param correctChar the character that should be present
 * @param foundChar the character that was read
 * @param row current row position of error
 * @param col current col position of error
 */
void raiseBoardError(int *lineNumber, char correctChar, char foundChar, int row, int col);

/**
 *  Move Error:
 * - board coordinates invalid
 * @param lineNumber of error
 * @param correctChar the character that should be present
 * @param foundChar the character that was read
 */
void raiseMoveError(int *lineNumber, char correctChar, char foundChar);


#endif