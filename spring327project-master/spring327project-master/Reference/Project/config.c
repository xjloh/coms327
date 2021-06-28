#include "headers/config.h"
#include "headers/errors.h"
#include "headers/helpers.h"

//#define DEBUG

Config setup;

void evaluateConfigFile(FILE *file) {
    char c;
    // first letter of keywords to look for
    char keywords[] = "RTBM";
    // keeps track of what keywords have been evaluated
    int keyword_index = 0;
    // keeps track of position in file
    int currentLine = 1;
    bool completedAllChecks = false;


    for (;;) {
        c = fgetc(file);
        // if end of file, quit
        if (feof(file)) {
            if (keyword_index != 3 || !completedAllChecks) {
                raiseFormattingError(&currentLine, "unexpected end of file");
            }
            break;
        }
        if (skipCharacter(c, &currentLine, file, true, true) ||
            (keyword_index == 3 && (c == '-' || c == '|' || c == '+'))) {
            continue;
        }
        if (c != keywords[keyword_index]) {
            switch (keyword_index) {
                case 0:
                    raiseFormattingError(&currentLine, "expecting 'RULES:'");
                    break;
                case 1:
                    raiseFormattingError(&currentLine, "expecting 'TURN:'");
                    break;
                case 2:
                    raiseFormattingError(&currentLine, "expecting 'BOARD:'");
                    break;
                case 3:
                    raiseFormattingError(&currentLine, "expecting 'MOVES:'");
                    break;
            }
        }
        switch (keyword_index) {
            case 0:
                validateRule(c, file, &currentLine);
                break;
            case 1:
                validateTurn(c, file, &currentLine);
                break;
            case 2:
                validateBoard(c, file, &currentLine);
                break;
            case 3:
                validateMoves(c, file, &currentLine);
                completedAllChecks = true;
                break;
            default:
                break;
        }
        keyword_index++;
        // all keywords evaluated
        if (keyword_index == 4) {
            break;
        }
    }
}

bool skipCharacter(char c, int *currentLine, FILE *file, bool skipSpaces, bool skipNewLine) {
    if (c == ' ' && skipSpaces) {
        return true;
    }
    // skips all characters from # till end of line
    if (c == '#') {
        while (c != '\n') {
            c = fgetc(file);
            if (feof(file)) {
                break;
            }
        }
    }
    if (c == '\n' && skipNewLine) {
        *currentLine += 1;
        return true;
    }
    if (c == '\t' || c == '\r') {
        return true;
    }
    return false;
}

void validateRule(char c, FILE *file, int *currentLine) {
    char keyword[] = "ULES:";
    char rule1[] = "capture";
    char rule2[] = "no capture";
    char rule3[] = "single jumps";
    char rule4[] = "multiple jumps";

    int index = 0;
    char captureRule[11] = "capture";
    char jumpRule[15] = "single jumps";
    int checkedRules = 0;
    bool ruleChecked = false;

    while (true) {
        // everything has been checked, then break
        if (ruleChecked && checkedRules == 2) {
            break;
        }
        c = fgetc(file);
        // don't skip spaces because rules are allowed to have one space
        if (skipCharacter(c, currentLine, file, false, true)) {
            continue;
        }
        if (feof(file)) {
            if (checkedRules < 2) {
                raiseFormattingError(currentLine, "unexpected end of file");
            }
            break;
        }
        // only skip the space if checking keyword
        if (c == ' ' && (ruleChecked || checkedRules == 0)) {
            continue;
        }
        // after one rule checked, reset values
        if (ruleChecked) {
            index = 0;
            checkedRules++;
            ruleChecked = false;
        }
        // check capture rule
        if (checkedRules == 1 && index == 0) {
            if (c != rule1[0]) {
                size_t destination_size = sizeof(captureRule);
                strncpy(captureRule, rule2, destination_size);
                captureRule[destination_size - 1] = '\0';
            }
        }
        // check jump rule
        if (checkedRules == 2 && index == 0) {
            if (c != rule3[0]) {
                size_t destination_size2 = sizeof(jumpRule);
                strncpy(jumpRule, rule4, destination_size2);
                jumpRule[destination_size2 - 1] = '\0';
            }
        }
        // error handling & completion of check logic
        switch (checkedRules) {
            case 0:
                if (c != keyword[index]) {
                    raiseFormattingError(currentLine, "expecting 'RULES:'");
                }
                if (index == strlen(keyword) - 1) {
                    ruleChecked = true;
                }
                break;
            case 1:
                if (c != captureRule[index]) {
                    raiseFormattingError(currentLine, "expecting 'capture' or 'no capture'");
                }
                if (index == strlen(captureRule) - 1) {
                    ruleChecked = true;
                }
                break;
            case 2:
                if (c != jumpRule[index]) {
                    raiseFormattingError(currentLine, "expecting 'single jumps' or 'multiple jumps'");
                }
                if (index == strlen(jumpRule) - 1) {
                    ruleChecked = true;
                }
                break;
            default:
                break;
        }
        index++;
    }

    // update setup structure
    size_t destination_size = sizeof(setup.jumpRule);
    strncpy(setup.jumpRule, jumpRule, destination_size);
    setup.jumpRule[destination_size - 1] = '\0';

    size_t destination_size2 = sizeof(setup.captureRule);
    strncpy(setup.captureRule, captureRule, destination_size2);
    setup.captureRule[destination_size2 - 1] = '\0';
}

void validateTurn(char c, FILE *file, int *currentLine) {
    char keyword[] = "URN:";
    char turnRed[] = "red";
    char turnBlack[] = "black";

    int index = 0;
    char turn[6] = "red";
    int checkedRules = 0;
    bool ruleChecked = false;


    while (true) {
        // if all rules check, break
        if (ruleChecked && checkedRules == 1) {
            break;
        }
        c = fgetc(file);
        // skip both spaces and new lines
        if (skipCharacter(c, currentLine, file, true, true)) {
            continue;
        }
        if (feof(file)) {
            if (checkedRules < 2) {
                raiseFormattingError(currentLine, "unexpected end of file");
            }
            break;
        }
        // once rule checked, reset values
        if (ruleChecked) {
            index = 0;
            checkedRules++;
            ruleChecked = false;
        }
        // check red/black
        if (checkedRules == 1 && index == 0) {
            if (c != turnRed[0]) {
                size_t destination_size = sizeof(turn);
                strncpy(turn, turnBlack, destination_size);
                turn[destination_size - 1] = '\0';
            }
        }
        // error handling & rule completion logic
        switch (checkedRules) {
            case 0:
                if (c != keyword[index]) {
                    raiseFormattingError(currentLine, "expecting 'TURN:'");
                }
                if (index == strlen(keyword) - 1) {
                    ruleChecked = true;
                }
                break;
            case 1:
                if (c != turn[index]) {
                    raiseFormattingError(currentLine, "expecting 'red' or 'black'");
                }
                if (index == strlen(turn) - 1) {
                    ruleChecked = true;
                }
                break;
            default:
                break;
        }
        index++;
    }

    // update setup structure
    size_t destination_size = sizeof(setup.boardConfig.turn);
    strncpy(setup.boardConfig.turn, turn, destination_size);
    setup.boardConfig.turn[destination_size - 1] = '\0';
}

void validateBoard(char c, FILE *file, int *currentLine) {
    setup.isFlipped = false;
    char keyword[] = "OARD:";
    int redPawns = 0;
    int redKings = 0;
    int blackPawns = 0;
    int blackKings = 0;

    int index = 0;
    bool checkKeyword = true;
    bool checkBoard = false;
    // position
    int col = 0;
    int row = 0;
    // expected next square value
    char nextSquare = '"';

    while (true) {
        if (col == 8) {
            col = 0;
            // alternating starting characters after every row
            if (row % 2 == 0) {
                if (setup.isFlipped) {
                    nextSquare = '"';
                } else {
                    nextSquare = '.';
                }
            } else {
                if (setup.isFlipped) {
                    nextSquare = '.';
                } else {
                    nextSquare = '"';
                }
            }
            row++;
            if (row == 8) {
                break;
            }
        }
        c = fgetc(file);
        // skip spaces and new lines
        if (skipCharacter(c, currentLine, file, true, true) ||
            (checkBoard && (c == '|' || c == '-' || c == '+'))) {
            continue;
        }
        if (feof(file)) {
            if (row != 8) {
                raiseFormattingError(currentLine, "unexpected end of file");
            }
            break;
        }
        // check keyword
        if (checkKeyword) {
            if (c != keyword[index]) {
                raiseFormattingError(currentLine, "expecting 'BOARD:'");
            }
            if (index == strlen(keyword) - 1) {
                checkKeyword = false;
                checkBoard = true;
            }
            index++;
        }
            // check board
        else if (checkBoard) {
            // logic for flipped board
            if (c != '"' && col == 0 && row == 0) {
                setup.isFlipped = true;
                nextSquare = '.';
            }
            // all letters should appear on black square
            // regardless of flipped board
            switch (c) {
                case 'R':
                    if (nextSquare == '.') {
                        redKings++;
                    } else {
                        raiseBoardError(currentLine, '"', c, row, col);
                    }
                    break;
                case 'r':
                    if (nextSquare == '.') {
                        redPawns++;
                    } else {
                        raiseBoardError(currentLine, '"', c, row, col);
                    }
                    break;
                case 'B':
                    if (nextSquare == '.') {
                        blackKings++;
                    } else {
                        raiseBoardError(currentLine, '"', c, row, col);
                    }
                    break;
                case 'b':
                    if (nextSquare == '.') {
                        blackPawns++;
                    } else {
                        raiseBoardError(currentLine, '"', c, row, col);
                    }
                    break;
                case '.':
                    if (nextSquare == '"') {
                        raiseBoardError(currentLine, '"', c, row, col);
                    }
                    break;
                case '"':
                    if (nextSquare == '.' && !setup.isFlipped) {
                        raiseBoardError(currentLine, '.', c, row, col);
                    }
                    break;
                default:
                    raiseBoardError(currentLine, 'i', c, row, col);
            }
            // switch the next square value from its current state
            if (nextSquare == '.') {
                nextSquare = '"';
            } else {
                nextSquare = '.';
            }
            // set board value
            setup.boardConfig.board[row][col] = c;
            col++;
        }
    }

    // update setup structure
    setup.boardConfig.blackKings = blackKings;
    setup.boardConfig.blackPawns = blackPawns;
    setup.boardConfig.redKings = redKings;
    setup.boardConfig.redPawns = redPawns;
}

void validateMoves(char c, FILE *file, int *currentLine) {
    char keyword[] = "OVES:";
    int index = 0;
    bool checkKeyword = true;
    bool checkMoves = false;

    int col;
    int row;
    int countMoves = 0;
    int countPositions = 0;
    Position position;
    Move move;
    move.positions = malloc((countPositions + 1) * sizeof(Position));
    if (!move.positions) {
        fprintf(stderr, "Malloc error on `move.positions`.\n");
        exit(1);
    }
    setup.moves = malloc((countMoves + 1) * sizeof(Move));
    if (!setup.moves) {
        fprintf(stderr, "Malloc error on `setup.moves`.\n");
        exit(1);
    }

    char expectedOrder[] = "->ln";

    bool skipSpaces = true;
    bool completedMove = false;
    bool skipNewLine = true;

    while (true) {
        c = fgetc(file);
        if (skipCharacter(c, currentLine, file, skipSpaces, skipNewLine)) {
            continue;
        }
        if (!checkKeyword && index == -1) {
            skipSpaces = false;
        }
        if (feof(file)) {
            if (!completedMove && index != -2) {
                raiseFormattingError(currentLine, "unexpected end of file");
            }
            if (!skipSpaces) {
                move.numPositions = countPositions;
                setup.moves[countMoves] = move;
                countMoves++;
                setup.numMoves = countMoves;
            }
            break;
        }
        // check keyword
        if (checkKeyword) {
            if (c != keyword[index]) {
                raiseFormattingError(currentLine, "expecting 'MOVES:'");
            }
            if (index == strlen(keyword) - 1) {
                checkMoves = true;
                checkKeyword = false;
                index = -2;
            } else {
                index++;
            }
        }
            // check move
        else if (checkMoves) {
            switch (c) {
                case '-':
                    // logic to ensure that a ln pair before dash
                    if (expectedOrder[index] != c || index < 0) {
                        if (index == -2) {
                            raiseMoveError(currentLine, 'l', c);
                        }
                        if (index == -1) {
                            raiseMoveError(currentLine, 'n', c);
                        }
                        raiseMoveError(currentLine, expectedOrder[index], c);
                    }
                    index++;
                    completedMove = false;
                    continue;
                case '>':
                    // logic to ensure that a ln pair before arrow
                    if (expectedOrder[index] != c) {
                        if (index == -2) {
                            raiseMoveError(currentLine, 'l', c);
                        }
                        if (index == -1) {
                            raiseMoveError(currentLine, 'n', c);
                        }
                        raiseMoveError(currentLine, expectedOrder[index], c);
                    }
                    index++;
                    completedMove = false;
                    continue;
                case ' ':
                case '\n':
                    // error handling when early end move when not completed
                    if (index != strlen(expectedOrder) && !completedMove) {
                        raiseMoveError(currentLine, expectedOrder[index], c);
                    }
                    // spaces and new lines = end of move
                    // reset values
                    index = -2;
                    move.numPositions = countPositions;
                    countPositions = 0;
#ifdef DEBUG
                printf("Position in Moves: %d\n", countMoves);
                printf("Values of Move: %d%d, %d%d\n",
                        move.positions[0].row, move.positions[0].col,
                        move.positions[1].row, move.positions[1].col);
#endif
                    setup.moves[countMoves] = move;
#ifdef DEBUG
                printf("Values of Moves: %d%d, %d%d\n",
                       setup.moves[countMoves].positions[0].row,  setup.moves[countMoves].positions[0].col,
                       setup.moves[countMoves].positions[1].row,  setup.moves[countMoves].positions[1].col);
#endif
                    countMoves++;
                    setup.numMoves = countMoves;
#ifdef DEBUG
                    printf("realloc in Moves: %d\n", countMoves+1);
#endif
                    setup.moves = realloc(setup.moves, (countMoves + 1) * sizeof(Move));
                    if (!setup.moves) {
                        fprintf(stderr, "Realloc error on `setup.moves`.\n");
                        exit(1);
                    }
#ifdef DEBUG
                    printf("Reset realloc Move: %d\n", countPositions+1);
#endif
                    move.positions = malloc((countPositions + 1) * sizeof(Position));
                    if (!move.positions) {
                        fprintf(stderr, "malloc error on `move.positions`.\n");
                        exit(1);
                    }
                    skipSpaces = true;
                    skipNewLine = true;
#ifdef DEBUG
                    printf("\nNew Move\n");
#endif
                    continue;
                default:
                    break;
            }
            // checking alpha characters within a-f
            if (isalpha(c) && (expectedOrder[index] == 'l' || index == -2)) {
                col = mapLetterToInt(c);
                position.col = col;
#ifdef DEBUG
                printf("Col: %d\n", position.col);
#endif
                if (col == -1) {
                    raiseMoveError(currentLine, 'l', c);
                }
                index++;
                skipNewLine = false;
                completedMove = false;
            }
                // checking int character within 1-8
            else if (isdigit(c) && (expectedOrder[index] == 'n' || index == -1)) {
                row = c - '0';
                position.row = boardRowToRow(row);
#ifdef DEBUG
                printf("Row: %d\n", position.row);
#endif
                if (row < 1 || row > 8) {
                    raiseMoveError(currentLine, 'n', c);
                }
                index++;
                if (index == strlen(expectedOrder)) {
                    index = 0;
                    completedMove = true;
#ifdef DEBUG
                    printf("realloc Move: %d\n", countPositions+1);
#endif
                    move.positions = realloc(move.positions, ((countPositions + 1) * sizeof(Position)));
                    if (!move.positions) {
                        fprintf(stderr, "Realloc error on `move.positions`.\n");
                        exit(1);
                    }
#ifdef DEBUG
                    printf("Position in Move: %d - %d:%d\n", countPositions, position.row, position.col);
#endif
                    move.positions[countPositions] = position;
                    countPositions++;
                } else {
#ifdef DEBUG
                    printf("realloc Move: %d\n", countPositions+1);
#endif
                    move.positions = realloc(move.positions, ((countPositions + 1) * sizeof(Position)));
                    if (!move.positions) {
                        fprintf(stderr, "Realloc error on `move.positions`.\n");
                        exit(1);
                    }
#ifdef DEBUG
                    printf("Position in Move: %d - %d:%d\n", countPositions, position.row, position.col);
#endif
                    move.positions[countPositions] = position;
                    countPositions++;
                }
            }
                // error handling for invalid first 2 characters of move
            else {
                if (index == -2) {
                    raiseMoveError(currentLine, 'l', c);
                }
                if (index == -1) {
                    raiseMoveError(currentLine, 'n', c);
                }
                raiseMoveError(currentLine, expectedOrder[index], c);
            }
        }
    }
    free(move.positions);
}

void printValidConfigInfo() {
    fprintf(stdout, "VALID INPUT\n"
                    "Initial configuration:\n"
                    "Turn: %s\n"
                    "Red: %d kings, %d pawns\n"
                    "Black: %d kings, %d pawns\n",
            setup.boardConfig.turn,
            setup.boardConfig.redKings, setup.boardConfig.redPawns,
            setup.boardConfig.blackKings, setup.boardConfig.blackPawns);
}