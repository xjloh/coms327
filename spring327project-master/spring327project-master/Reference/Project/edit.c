#include "headers/helpers.h"
#include "headers/render.h"
#include "headers/moves.h"
#include <termbox.h>

Config setup;

// #define DEBUG

int main(int argc, char *argv[]){
    BoardConfig copy;
    FILE* configFile;
    char *configFileName = argv[1];

    configFile = fopen(configFileName, "r");
    if (!configFile) {
        fprintf(stderr, "'%s' can't be opened.\n", configFileName);
         exit(1);
    }

    evaluateConfigFile(configFile);
    fclose(configFile);
    setup.invalidMoveIndex = -1;
    setup.fileName = configFileName;
    setup.showMessages = false;

    // create copy of original board
    copyBoardConfig(&setup.boardConfig, &copy);

    // find any invalid moves
    executeMoves(setup.numMoves, true, true);
    // revert back to original board
    copyBoardConfig(&copy, &setup.boardConfig);

    // cursor location
    int cy = 6;
    int cx = 0;
    int col = 0;
    int row = 0;

    char type;
    char* moveText;
    struct tb_event event;

    // editing moves
    bool editMode = false;
    int editIndex = 0;
    int posIndex = 0;
    Position newPos;
    Move newMove;
    newMove.numPositions = 0;
    newMove.positions = malloc(sizeof(Position));
    if(!newMove.positions){
        fprintf(stderr, "Malloc failed on newMove.positions\n");
        exit(1);
    }

    tb_init(); // create termbox

    // range of moves to show
    int start = 0;
    int end = (tb_height() - 6) * 2;
    int index = (row * 2) + col;

    renderInstructions();
    renderBoard();
    renderMoves(start, end);
    renderMoveLocationFileName(index);

    // highlight first move
    moveText = getMoveText(newMove, index);
    type = getMoveType(index);
    highlightMove(cx, cy, moveText, type, false);

    int CURSOR_LENGTH = (tb_width() / 2) / 2;
    int CURSOR_HEIGHT = 1;

    for (;;) {
        tb_present(); // show
        tb_poll_event(&event);

        // esc = exit
        if(event.key == TB_KEY_ESC){
            break;
        }

        index = (row * 2) + col;

        switch (event.key) {
            // edit move
            case TB_KEY_CTRL_E:
                if(editMode){
                    break;
                }
                editMode = true;
                newPos.col = 0;
                newPos.row = 7;
                break;
            // save move
            case TB_KEY_CTRL_S:
                if(posIndex < 2){
                    break;
                }

                // append a move
                if(index == setup.numMoves){
                    setup.numMoves++;
                    setup.moves = realloc(setup.moves, setup.numMoves * sizeof(Move));
                    if(!setup.moves){
                        fprintf(stderr, "Realloc failed on setup.moves\n");
                        exit(1);
                    }
                    setup.moves[index] = newMove;
                }
                // change a move
                else {
                    setup.moves[index] = newMove;
                }

                // handles updating the input file contents
                copyBoardConfig(&copy, &setup.boardConfig);
                updateInputFile();

                // reset values
                editMode = false;
                editIndex = 0;
                posIndex = 0;
                newMove.numPositions = 0;
                newMove.positions = malloc(sizeof(Position));
                if(!newMove.positions){
                    fprintf(stderr, "Malloc failed on newMove.positions\n");
                    exit(1);
                }

                copyBoardConfig(&copy, &setup.boardConfig);
                setup.invalidMoveIndex = -1;
                executeMoves(setup.numMoves, true, true);
                clearMoveLines();
                renderMoves(start, end);
                copyBoardConfig(&copy, &setup.boardConfig);
                break;
            // add position to move
            case TB_KEY_CTRL_N:
                if(editMode){
                    // add position to move
                    newMove.positions[posIndex++] = newPos;
                    newMove.positions = realloc(newMove.positions, (posIndex + 1) * sizeof(Position));
                    if(!newMove.positions){
                        fprintf(stderr, "Realloc failed on newMove.positions\n");
                        exit(1);
                    }
                    newMove.numPositions++;
                }
                break;
            // cancel edit
            case TB_KEY_CTRL_C:
                if(editMode){
                    // reset values
                    editMode = false;
                    editIndex = 0;
                    posIndex = 0;
                    newMove.numPositions = 0;
                    newMove.positions = malloc(sizeof(Position));
                    if(!newMove.positions){
                        fprintf(stderr, "Malloc failed on newMove.positions\n");
                        exit(1);
                    }

                }
                break;
            case TB_KEY_ARROW_LEFT:
                // edit col
                if(editMode){
                    editIndex = 0;
                    break;
                }

                // can't go above cy = 6
                if(cy == 6 && cx == 0){
                    break;
                }

                highlightMove(cx, cy, moveText, type, true);

                if(cx == 0){
                    cy -= CURSOR_HEIGHT;
                    row --;
                    cx += CURSOR_LENGTH;
                    col = 1;
                }
                else {
                    cx = 0;
                    col = 0;
                }
                break;
            case TB_KEY_ARROW_RIGHT:
                // edit row
                if(editMode){
                    editIndex = 1;
                    break;
                }

                // don't move onto edit line
                if(cy == (tb_height() - CURSOR_HEIGHT - 1) && cx == CURSOR_LENGTH){
                    break;
                }

                // end of possible moves
                if(index == setup.numMoves){
                    break;
                }

                highlightMove(cx, cy, moveText, type, true);

                if(cx == CURSOR_LENGTH){
                    cy += CURSOR_HEIGHT;
                    row ++;
                    cx = 0;
                    col = 0;
                }
                else {
                    cx += CURSOR_LENGTH;
                    col = 1;
                }
                break;
            case TB_KEY_ARROW_DOWN:
                if(editMode){
                    // col
                    if(editIndex == 0){
                        if(newPos.col > 0){
                            newPos.col--;
                        }
                    }
                    // row
                    else {
                        if(newPos.row < 7) {
                            newPos.row++;
                        }
                    }
                    break;
                }

                if(end > (setup.numMoves + 2)){
                    break;
                }

                row++;
                start += 2;
                end += 2;
                break;
            case TB_KEY_ARROW_UP:
                if(editMode){
                    // col
                    if(editIndex == 0){
                        if(newPos.col < 7){
                            newPos.col++;
                        }
                    }
                    // row
                    else {
                        if(newPos.row > 0) {
                            newPos.row--;
                        }
                    }
                    break;
                }

                if(start == 0){
                    break;
                }
                row --;
                start -= 2;
                end -= 2;
                break;
        }

        if(editMode){
            showEditMove(newMove, newPos, editIndex);
        }
        else {
            clearEditLine();
        }

        index = (row * 2) + col;

        copyBoardConfig(&copy, &setup.boardConfig);
        executeMoves(index, true, false);
        renderBoard();
        renderMoveLocationFileName(index);

        clearMoveLines();
        renderMoves(start, end);

        moveText = getMoveText(newMove, index);
        type = getMoveType(index);
        highlightMove(cx, cy, moveText, type, false);
    }

    // clean up
    tb_shutdown();
    free(moveText);
    free(newMove.positions);
    deallocateChangeInfo();
    return 0;
}
