#include "headers/config.h"
#include "headers/helpers.h"
#include "headers/moves.h"
#include "headers/render.h"
#include <termbox.h>

Config setup;
// #define DEBUG

void renderInstructions(){
    struct tb_cell cell;
    int cx = 0;
    int cy = 1;

    char* movementAndQuit = "left, right: move cursor  esc (quit)";
    char* scrollAndSave = "up, down: scroll  ctrl+s (save)";
    char* editNextCancel = "ctrl+: e (edit) n (next) c (cancel)";
    char* red = "Red";
    char* black = "Black";
    int k;

    cell.bg = TB_DEFAULT;
    cell.fg = TB_WHITE;

    // move cursor
    for(k = 0; k < strlen(movementAndQuit); k++){
        cell.ch = movementAndQuit[k];
        tb_put_cell(cx, cy, &cell);
        cx++;
    }

    cy++;
    cx = 0;

    // scroll and save
    for(k = 0; k < strlen(scrollAndSave); k++){
        cell.ch = scrollAndSave[k];
        tb_put_cell(cx, cy, &cell);
        cx++;
    }

    cy++;
    cx = 0;

    // edit, next, cancel
    for(k = 0; k < strlen(editNextCancel); k++){
        cell.ch = editNextCancel[k];
        tb_put_cell(cx, cy, &cell);
        cx++;
    }

    cy = 5;
    cx = 0;

    if(isRed(setup.boardConfig.turn)){
        // red
        for(k = 0; k < strlen(red); k++){
            cell.ch = red[k];
            tb_put_cell(cx, cy, &cell);
            cx ++;
        }

        cx = (tb_width() / 2) - strlen(black) - 1;


        // black
        for(k = 0; k < strlen(black); k++){
            cell.ch = black[k];
            tb_put_cell(cx, cy, &cell);
            cx++;
        }
    }

    else {
        // black
        for(k = 0; k < strlen(black); k++){
            cell.ch = black[k];
            tb_put_cell(cx, cy, &cell);
            cx++;
        }

        cx = (tb_width() / 2) - strlen(red) - 1;

        for(k = 0; k < strlen(red); k++){
            cell.ch = red[k];
            tb_put_cell(cx, cy, &cell);
            cx ++;
        }
    }

    tb_present();
}

void renderBoard(){
    // constants
    char RED_KING = 'R';
    char RED_PAWN = 'r';
    char BlACK_KING = 'B';
    char BLACK_PAWN = 'b';
    char EMPTY = ' ';

    int CELL_WIDTH = 5;
    int CELL_HEIGHT = 3;

    unsigned int r; // row
    unsigned int c; // col
    unsigned int w; // width
    unsigned int h; // height

    char currentPiece;
    struct tb_cell cell;
    bool hasPiece = false;

    int cy = 1;
    int cx, tempX, tempY;

    for(r = 0; r < 8; r++){
        cx = tb_width() / 2;
        tempY = cy;
        for(c = 0; c < 8; c++){
            tempX = cx;
            currentPiece = setup.boardConfig.board[r][c];
            hasPiece = false;
            switch(currentPiece){
                case '.':
                    cell.bg = TB_WHITE;
                    cell.fg = TB_DEFAULT;
                    break;
                case '"':
                    cell.bg = TB_RED;
                    cell.fg = TB_DEFAULT;
                    break;
                default:
                    cell.bg = TB_WHITE;
                    if(isRed(pieceToTurn(currentPiece))){
                        cell.fg = TB_RED;
                    }
                    else {
                        cell.fg = TB_BLACK;
                    }
                    hasPiece = true;
            }
            for(h = 0; h < CELL_HEIGHT; h++){
                for(w = 0; w < CELL_WIDTH; w++){
                    if(hasPiece && w == 2 && h == 1){
                        cell.ch = currentPiece;
                    }
                    else {
                        cell.ch = EMPTY;
                    }
                    tb_put_cell(tempX + w, tempY + h, &cell);
                }
            }
            cx += CELL_WIDTH;
        }
        cy += CELL_HEIGHT;
    }
}

void highlightMove(int cx, int cy, char* moveText, char type, bool reset){
    int i;
    int CURSOR_LENGTH = (tb_width() / 2) / 2;
    struct tb_cell cell;

    for(i = 0; i < CURSOR_LENGTH; i++){
        switch(type){
            case 'v':
                if(reset){
                    cell.fg = TB_GREEN;
                }
                else {
                    cell.fg = TB_BLACK;
                    cell.bg = TB_GREEN;
                }
                break;
            case 'i':
                if(reset){
                    cell.fg = TB_RED;
                }
                else {
                    cell.fg = TB_WHITE;
                    cell.bg = TB_RED;
                }
                break;
            case 'u':
                if(reset) {
                    cell.fg = TB_WHITE;
                }
                else {
                    cell.fg = TB_BLACK;
                    cell.bg = TB_WHITE;
                }
                break;
            case 'e':
                cell.bg = TB_WHITE;
                cell.fg = TB_DEFAULT;
                break;
        }

        if(reset){
            cell.bg = TB_DEFAULT;
        }

        // print move
        if(i < strlen(moveText)) {
            cell.ch = moveText[i];
        }
        else {
            cell.ch = ' ';
        }

        tb_put_cell(cx++, cy, &cell);
    }
}

void renderMoveLocationFileName(int index){
    struct tb_cell cell;
    int cx = 0;
    int i;
    int tempIndex = ++index;
    int tempTotal = setup.numMoves + 1;

    // clear line
    for(i = 0; i < tb_width(); i++){
        cell.ch = ' ';
        cell.bg = TB_DEFAULT;
        cell.fg = TB_DEFAULT;
        tb_put_cell(i, 0, &cell);
    }

    cell.bg = TB_CYAN;
    cell.fg = TB_BLACK;

    int count = 0;
    while(tempIndex != 0){
        tempIndex /= 10;
        count++;
    }

    char location[count];
    sprintf(location, "%d", index);
    for(i = 0; i < strlen(location); i++){
        cell.ch = location[i];
        tb_put_cell(cx++, 0, &cell);
    }

    cell.ch = '/';
    tb_put_cell(cx++, 0, &cell);

    count = 0;
    while(tempTotal != 0){
        tempTotal /= 10;
        count++;
    }

    char totalMoves[count];
    sprintf(totalMoves, "%d", (setup.numMoves + 1));
    for(i = 0; i < strlen(totalMoves); i++){
        cell.ch = totalMoves[i];
        tb_put_cell(cx++, 0, &cell);
    }

    // space between move location and file name
    for(i = 0; i < 3; i++){
        cell.ch = ' ';
        tb_put_cell(cx++, 0, &cell);
    }

    for(i = 0; cx < tb_width(); i++){
        cell.ch = ' ';
        if(i < strlen(setup.fileName)) {
            cell.ch = setup.fileName[i];
        }
        tb_put_cell(cx++, 0, &cell);
    }

}

char* getMoveText(Move move, int index){
    int textSize = 0;
    char* moveText = malloc(sizeof(char));
    if(!moveText){
        fprintf(stderr, "Malloc failed on moveText\n");
        exit(1);
    }
    unsigned i;

    if(index == setup.numMoves){
        moveText[0] = ' ';
        return moveText;
    }

    if(index != -1){
        move = setup.moves[index];
    }

    // map to valid board move
    for (i = 0; i < move.numPositions; i++) {
        if((i == 5 && index != -1) || (i == 9 && index == -1)){
            moveText[textSize++] = '.';
            moveText = realloc(moveText, (textSize + 1) * sizeof(char));
            if(!moveText){
                fprintf(stderr, "Realloc failed on moveText\n");
                exit(1);
            }
            moveText[textSize++] = '.';
            break;
        }
        char col = mapIntToLetter(move.positions[i].col);
        int row = rowToBoardRow(move.positions[i].row);
        if (i == 0) {
            moveText[textSize++] = col;
            moveText = realloc(moveText, (textSize + 1) * sizeof(char));
            if(!moveText){
                fprintf(stderr, "Realloc failed on moveText\n");
                exit(1);
            }
            moveText[textSize++] = row + '0';
        } else {
            moveText[textSize++] = '-';
            moveText = realloc(moveText, (textSize + 4) * sizeof(char));
            if(!moveText){
                fprintf(stderr, "Realloc failed on moveText\n");
                exit(1);
            }
            moveText[textSize++] = '>';
            moveText[textSize++] = col;
            moveText[textSize++] = row + '0';
        }
    }
    return moveText;
}

char getMoveType(int index){
    // empty cell
    if(index == setup.numMoves){
        return 'e';
    }

    // no invalid moves
    if(setup.invalidMoveIndex == -1){
        return 'v';
    }

    // invalid cell
    if(index == setup.invalidMoveIndex){
        return 'i';
    }
    // unknown cell
    if(index > setup.invalidMoveIndex){
        return 'u';
    }
    // valid cell
    return 'v';
}

void renderMoves(int start, int end){
    int i, k;
    int CURSOR_LENGTH = (tb_width() / 2) / 2;
    char* moveText;
    char moveType;
    struct tb_cell cell;
    int cx = 0;
    int cy = 5;
    Move newMove;

    if(end > setup.numMoves){
        end = setup.numMoves;
    }

    for(i = start; i < end; i++){
        moveText = getMoveText(newMove, i);
        moveType = getMoveType(i);
        cell.bg = TB_DEFAULT;

        if(i % 2 == 0){
            cx = 0;
            cy++;
        }

        // don't print on last line
        if(cy == (tb_height() - 1)){
            break;
        }

        switch(moveType){
            case 'i':
                cell.fg = TB_RED;
                break;
            case 'v':
                cell.fg = TB_GREEN;
                break;
            case 'u':
                cell.fg = TB_WHITE;
                break;
        }
        for(k = 0; k < strlen(moveText); k++){
            cell.ch = moveText[k];
            tb_put_cell(cx++, cy, &cell);
        }
        cx = CURSOR_LENGTH;
    }
}

void clearEditLine(){
    int i;
    struct tb_cell cell;
    for(i = 0; i < tb_width() / 2; i++){
        cell.ch = ' ';
        cell.bg = TB_DEFAULT;
        cell.fg = TB_DEFAULT;
        tb_put_cell(i, tb_height()-1, &cell);
    }
}

void showEditMove(Move move, Position newPos, int editIndex){
    char* moveText = getMoveText(move, -1);
    int cx = 0;
    struct tb_cell cell;
    cell.fg = TB_CYAN;
    cell.bg = TB_DEFAULT;
    int k;

    clearEditLine();

    for(k = 0; k < strlen(moveText); k++){
        cell.ch = moveText[k];
        tb_put_cell(cx++, tb_height() -1, &cell);
    }

    cx++;

    cell.fg = TB_RED;
    if(editIndex == 0){
        cell.bg = TB_YELLOW;
    }
    else {
        cell.bg = TB_DEFAULT;
    }
    cell.ch = mapIntToLetter(newPos.col);
    tb_put_cell(cx++, tb_height() -1, &cell);

    if(editIndex == 1){
        cell.bg = TB_YELLOW;
    }
    else {
        cell.bg = TB_DEFAULT;
    }
    cell.ch = rowToBoardRow(newPos.row) + '0';
    tb_put_cell(cx++, tb_height() -1, &cell);
}

void clearMoveLines(){
    int i, k;
    struct tb_cell cell;
    cell.ch = ' ';
    cell.bg = TB_DEFAULT;
    cell.fg = TB_DEFAULT;

    for(i = 0; i < tb_width() / 2; i++){
        for(k = 6; k < tb_height() - 1; k++){
            tb_put_cell(i, k, &cell);
        }
    }
}

void updateInputFile(){
    FILE* f = fopen(setup.fileName, "w");
    if (!f) {
        fprintf(stderr, "'%s' can't be opened.\n", setup.fileName);
        exit(1);
    }

    fprintf(f, "RULES:\n\t%s\n\t%s\n", setup.captureRule, setup.jumpRule);
    fprintf(f, "TURN:\n\t%s\n", setup.boardConfig.turn);

    fprintf(f, "BOARD:\n");
    exportExchangeBoard(f);

    fprintf(f, "MOVES:\n");
    exportAllMoves(f);
    fclose(f);
}
