#include "headers/rank.h"
#include "headers/moves.h"
#include "headers/config.h"
#include "headers/helpers.h"

//#define DEBUG
RankScore rankScore;
Config setup;

#define UG
void calculateRank() {
    findOriginMove(0);
    if(strcmp(setup.captureRule, "capture") == 0) {
        filterCaptureMoves();
    }
    setDeepestScores();
    setNonDeepestScores();
#ifdef UG
    unsigned int k;
    PossibleMoves currentMove;
    for(k = 0; k < rankScore.numPossibleMoves; k++){
        currentMove = rankScore.possibleMoves[k];
        printf("Depth: %d - %c%d\n", currentMove.depthLevel, mapIntToLetter(currentMove.move.positions->col), rowToBoardRow(currentMove.move.positions->row));
    }
#endif
    printPossibleMoves();
    deallocateRankMoves();
}

void findOriginMove(int depth) {
    if (depth > rankScore.depth) {
        return;
    }
    int r;
    int c;
    char piece;
    char *turn;
    Position position;
    Position* piecePositions = malloc(sizeof(Position));
    if(!piecePositions){
        fprintf(stderr, "Malloc failed on piecePositions.\n");
        exit(1);
    }
    int numPositions = 0;

    for (r = 7; r >= 0; r--) {
        for (c = 0; c < 8; c++) {
            piece = setup.boardConfig.board[r][c];
            if (piece == '.' || piece == '"') {
                continue;
            }
            turn = pieceToTurn(piece);
            if (strcmp(turn, setup.boardConfig.turn) != 0) {
                continue;
            }
            position.col = c;
            position.row = r;

            piecePositions[numPositions++] = position;
            piecePositions = realloc(piecePositions, (numPositions+1) * sizeof(Position));
            if(!piecePositions){
                fprintf(stderr, "Realloc failed on piecePositions.\n");
                exit(1);
            }
#ifdef DEBUG
            printf("Col: %d Row: %d\n", position.col, position.row);
            printf("%c%d\n", mapIntToLetter(position.col), rowToBoardRow(position.row));
#endif
        }
    }
    alphabetize(piecePositions, numPositions);
    int i;
    for(i = 0; i < numPositions; i++){
        position = piecePositions[i];
        piece = setup.boardConfig.board[position.row][position.col];
#ifdef DEBUG
        printf("%c%d\n", mapIntToLetter(position.col), rowToBoardRow(position.row));
#endif
        findPossibleMoves(position, piece, depth);
    }
    free(piecePositions);
}

void alphabetize(Position* piecePositions, int numPositions){
#ifdef DEBUG
    printf("\nBefore Alpha\n");
    int g;
    Position p;
    for(g = 0; g < numPositions; g++){
        p = piecePositions[g];
        printf("%c%d ", mapIntToLetter(p.col), rowToBoardRow(p.row));
    }
#endif
    int i;
    int k;
    Position temp;
#ifdef DEBUG
    for(i = 0; i < numPositions; i++) {
        printf("%c%d\n", mapIntToLetter(piecePositions[i].col), rowToBoardRow(piecePositions[i].row));
    }
    printf("\n\n");
#endif
    // sort by cols
    for(i = 0; i < numPositions-1; i++){
        for(k = i+1; k < numPositions; k++) {
            if (piecePositions[i].col > piecePositions[k].col ) {
                temp = piecePositions[i];
                piecePositions[i] = piecePositions[k]; // current = next
                piecePositions[k] = temp; // next = temp
            }
        }
    }

    // sorts by rows
    for(i = 0; i < numPositions-1; i++){
        for(k = i+1; k < numPositions; k++) {
            if (piecePositions[i].col == piecePositions[k].col) {
                if (rowToBoardRow(piecePositions[k].row) < rowToBoardRow(piecePositions[i].row)) {
                    temp = piecePositions[i];
                    piecePositions[i] = piecePositions[k]; // current = next
                    piecePositions[k] = temp; // next = temp
                }
            }
        }
    }

#ifdef DEBUG
    printf("\nAfter Alpha\n");
    int j;
    Position p2;
    for(j = 0; j < numPositions; j++){
        p2 = piecePositions[j];
        printf("%c%d ", mapIntToLetter(p2.col), rowToBoardRow(p2.row));
    }
#endif
}

void filterCaptureMoves(){
    unsigned int i;
    int d;
    PossibleMoves currentMove;
    int* jumpOnlyDepths = malloc(sizeof(int));
    int numJumpOnlyDepths = 0;
    int depthBounds = rankScore.depth;
    if(depthBounds == 0){
        depthBounds = 1;
    }

    for(d = 0; d < depthBounds; d++){
        for(i = 0; i < rankScore.numCaptureMoves; i++){
            currentMove = rankScore.captureMoves[i];
            if(currentMove.isJump){
                if(inIntList(jumpOnlyDepths, numJumpOnlyDepths, d)){
                    continue;
                }
                jumpOnlyDepths[numJumpOnlyDepths++]  = d;
                jumpOnlyDepths = realloc(jumpOnlyDepths, numJumpOnlyDepths * sizeof(int));
                if(!jumpOnlyDepths){
                    fprintf(stderr, "Realloc failed on jumpOnlyDepths.\n");
                    exit(1);
                }
            }
        }
    }
#ifdef DEBUG
    unsigned int h;
    for(h = 0; h < numJumpOnlyDepths; h++){
        printf("%d ", jumpOnlyDepths[h]);
    }
    printf("\n");
#endif

    int skipMove = -1;

    for(i = 0; i < rankScore.numCaptureMoves; i++){
        currentMove = rankScore.captureMoves[i];
        if(!currentMove.isJump) {
            if (inIntList(jumpOnlyDepths, numJumpOnlyDepths, currentMove.depthLevel)) {
#ifdef DEBUG
                printf("Skipping: ");
                printf("%c%d->%c%d\n", mapIntToLetter(currentMove.move.positions[0].col), rowToBoardRow(currentMove.move.positions[0].row),
                       mapIntToLetter(currentMove.move.positions[1].col), rowToBoardRow(currentMove.move.positions[1].row));
#endif
                skipMove = currentMove.depthLevel;
                continue;
            }
        }
        if(skipMove != -1 && currentMove.depthLevel > skipMove){
#ifdef UG
            printf("Skipping: ");
            printf("%c%d->%c%d\n", mapIntToLetter(currentMove.move.positions[0].col), rowToBoardRow(currentMove.move.positions[0].row),
                   mapIntToLetter(currentMove.move.positions[1].col), rowToBoardRow(currentMove.move.positions[1].row));
#endif
            continue;
        }

        rankScore.possibleMoves = realloc(rankScore.possibleMoves,
                                          (rankScore.numPossibleMoves + 1) * sizeof(PossibleMoves));
        if (!rankScore.possibleMoves) {
            fprintf(stderr, "Realloc failed on rankScore.possibleMoves");
            exit(1);
        }
        rankScore.possibleMoves[rankScore.numPossibleMoves] = currentMove;
        rankScore.numPossibleMoves++;

        skipMove = -1;
    }

    rankScore.numCaptureMoves = 0;
    rankScore.captureMoves = malloc(sizeof(PossibleMoves));
}

/**
 * left before right (A - left to H - right
 * left (jump before single)
 * right (single before jump)
 * black before red (black -boardRow, red + boardRow)
 */
void findPossibleMoves(Position origin, char piece, int depth) {
#ifdef DEBUG
    printf("Depth in findPossibleMoves: %d\n\n", depth);
#endif
    bool isRedTurn = isRed(setup.boardConfig.turn);
    bool isRedKing = false;
    bool isBlackKing = false;

    // determine if piece is a king
    if (piece == 'B') {
        isBlackKing = true;
    }
    if (piece == 'R') {
        isRedKing = true;
    }

    BoardConfig copy;
    Move* captureMoves = malloc(sizeof(Move)); // used for capture
    if(!captureMoves){
        fprintf(stderr, "Malloc failed on captureMoves.\n");
        exit(1);
    }
    captureMoves->numPositions = 0;
    int numPositions = 2; // used for multiple jumps
    int endCol;
    int endRow;
    bool captureEnabled = false;
    if(strcmp(setup.captureRule, "capture") == 0){
        captureEnabled = true;
    }

    Position endPosition;
    copyBoardConfig(&setup.boardConfig, &copy);
    if (captureEnabled) {
        findCapturePositions(captureMoves, origin.row, origin.col, isRedTurn, false);
#ifdef DEBUG
        printf("Capture: ");
        int k;
        Position p;
        for(k = 0; k < captureMoves->numPositions; k++){
            p = captureMoves->positions[k];
            printf("%c%d ", mapIntToLetter(p.col), rowToBoardRow(p.row));
        }
        printf("\n");
#endif
    }

#ifdef DEBUG
    printf("Depth: %d Setup.Turn: %s\n", depth, setup.boardConfig.turn);
#endif

    // black pawns and red kings
    if (!isRedTurn || isRedKing) {
        // jump move (left)
        endRow = origin.row + 2;
        endPosition.row = endRow;

        endCol = origin.col - 2;
        if (validPositionToMove(origin.row, origin.col, endRow, endCol, isRedTurn, captureMoves, numPositions)) {
            endPosition.col = endCol;
            handleValidMove(origin, endPosition, piece, isRedTurn);

            addPossibleMove(origin, endPosition, depth, captureEnabled);
            findOriginMove(depth + 1);
            copyBoardConfig(&copy, &setup.boardConfig);
#ifdef DEBUG
            printf("Depth: %d after jump left move black\n", depth);
#endif
        }
    }

    // red pawns and black kings
    if (isRedTurn || isBlackKing) {
        // jump move (left)
        endRow = origin.row - 2;
        endPosition.row = endRow;

        endCol = origin.col - 2;
        if (validPositionToMove(origin.row, origin.col, endRow, endCol, isRedTurn, captureMoves, numPositions)) {
            endPosition.col = endCol;
            handleValidMove(origin, endPosition, piece, isRedTurn);

            addPossibleMove(origin, endPosition, depth, captureEnabled);
            findOriginMove(depth + 1);
            copyBoardConfig(&copy, &setup.boardConfig);
#ifdef DEBUG
            printf("Depth: %d after jump left move red\n", depth);
#endif
        }
    }

    // black pawns and red kings
    if (!isRedTurn || isRedKing) {
        // single move (left)
        endRow = origin.row + 1;
        endPosition.row = endRow;

        endCol = origin.col - 1;
        if (validPositionToMove(origin.row, origin.col, endRow, endCol, isRedTurn, captureMoves, numPositions)) {
            endPosition.col = endCol;
            handleValidMove(origin, endPosition, piece, isRedTurn);

            addPossibleMove(origin, endPosition, depth, false);
            findOriginMove(depth + 1);
            copyBoardConfig(&copy, &setup.boardConfig);
#ifdef DEBUG
            printf("Depth: %d after single left move black\n", depth);
#endif
        }
    }

    // red pawns and black kings
    if (isRedTurn || isBlackKing) {
        // single move (left)
        endRow = origin.row - 1;
        endPosition.row = endRow;

        endCol = origin.col - 1;
        if (validPositionToMove(origin.row, origin.col, endRow, endCol, isRedTurn, captureMoves, numPositions)) {
            endPosition.col = endCol;
            handleValidMove(origin, endPosition, piece, isRedTurn);

            addPossibleMove(origin, endPosition, depth, false);
            findOriginMove(depth + 1);
            copyBoardConfig(&copy, &setup.boardConfig);
#ifdef DEBUG
            printf("Depth: %d after single left move red\n", depth);
#endif
        }
    }

    // black pawns and red kings
    if (!isRedTurn || isRedKing) {
        // single move (right)
        endRow = origin.row + 1;
        endPosition.row = endRow;

        endCol = origin.col + 1;
        if (validPositionToMove(origin.row, origin.col, endRow, endCol, isRedTurn, captureMoves, numPositions)) {
            endPosition.col = endCol;
            handleValidMove(origin, endPosition, piece, isRedTurn);

            addPossibleMove(origin, endPosition, depth, false);
            findOriginMove(depth + 1);
            copyBoardConfig(&copy, &setup.boardConfig);
#ifdef DEBUG
            printf("Depth: %d after single right move black\n", depth);
#endif
        }
    }

    // red pawns and black kings
    if (isRedTurn || isBlackKing) {
        // single move (right)
        endRow = origin.row - 1;
        endPosition.row = endRow;

        endCol = origin.col + 1;
        if (validPositionToMove(origin.row, origin.col, endRow, endCol, isRedTurn, captureMoves, numPositions)) {
            endPosition.col = endCol;
            handleValidMove(origin, endPosition, piece, isRedTurn);

            addPossibleMove(origin, endPosition, depth, false);
            findOriginMove(depth + 1);
            copyBoardConfig(&copy, &setup.boardConfig);
#ifdef DEBUG
            printf("Depth: %d after single right move red\n", depth);
#endif
        }
    }

    // black pawns and red kings
    if (!isRedTurn || isRedKing) {
        // jump move (right)
        endRow = origin.row + 2;
        endPosition.row = endRow;

        endCol = origin.col + 2;
        if (validPositionToMove(origin.row, origin.col, endRow, endCol, isRedTurn, captureMoves, numPositions)) {
            endPosition.col = endCol;
            handleValidMove(origin, endPosition, piece, isRedTurn);

            addPossibleMove(origin, endPosition, depth, captureEnabled);
            findOriginMove(depth + 1);
            copyBoardConfig(&copy, &setup.boardConfig);
#ifdef DEBUG
            printf("Depth: %d after jump right move black\n", depth);
#endif
        }
    }

    // red pawns and black kings
    if (isRedTurn || isBlackKing) {
        // jump move (right)
        endRow = origin.row - 2;
        endPosition.row = endRow;

        endCol = origin.col + 2;
        if (validPositionToMove(origin.row, origin.col, endRow, endCol, isRedTurn, captureMoves, numPositions)) {
            endPosition.col = endCol;
            handleValidMove(origin, endPosition, piece, isRedTurn);

            addPossibleMove(origin, endPosition, depth, captureEnabled);
            findOriginMove(depth + 1);
            copyBoardConfig(&copy, &setup.boardConfig);
#ifdef DEBUG
            printf("Depth: %d after jump right move red\n", depth);
#endif
        }
    }
#ifdef DEBUG
    printf("Possible Moves: %d\n", rankScore.numPossibleMoves);
#endif
    free(captureMoves);
}

void handleValidMove(Position origin, Position dest, char piece, bool isRedTurn) {
    // update start position with blank black space
    setup.boardConfig.board[origin.row][origin.col] = '.';

    if (isRedTurn && dest.row == 0 && piece == 'r') {
        setup.boardConfig.redKings++;
        setup.boardConfig.redPawns--;
        piece = 'R';
    }
    if (!isRedTurn && dest.row == 7 && piece == 'b') {
        setup.boardConfig.blackKings++;
        setup.boardConfig.blackPawns--;
        piece = 'B';
    }

    // set new move
    setup.boardConfig.board[dest.row][dest.col] = piece;
}

void printPossibleMoves() {
    int i = 0;
    PossibleMoves *possibleMove;
    PossibleMoves nextMove;
    PossibleMoves toBePrinted;
    int depth = 0;

    int numNotPrinted = 0;
    int *notPrinted = malloc(sizeof(int));
    if (!notPrinted) {
        fprintf(stderr, "Malloc failed on notPrinted\n");
        exit(1);
    }

    for (i = 0; i < rankScore.numPossibleMoves; i++) {
#ifdef DEBUG
        printf("%d ", i);
#endif
        possibleMove = &rankScore.possibleMoves[i];
        depth = possibleMove->depthLevel;

        if (rankScore.isVerbose) {
#ifdef DEBUG
            printf("Depth: %d \n", depth);
#endif
            if (depth != rankScore.depth) {
                printQuestionMarkMove(*possibleMove);

                // handle opponent has no moves
                if ((i + 1) < rankScore.numPossibleMoves &&
                    rankScore.possibleMoves[i + 1].depthLevel == possibleMove->depthLevel) {
                    printPeriodMove(*possibleMove);
                }
                else if ((i+1) == rankScore.numPossibleMoves){
                    printPeriodMove(*possibleMove);
                    printNotPrintedList(notPrinted, numNotPrinted);
                }
                else {
                    // add to notPrinted stack
                    notPrinted[numNotPrinted++] = i;
                    notPrinted = realloc(notPrinted, (numNotPrinted + 1) * sizeof(int));
                    if (!notPrinted) {
                        fprintf(stderr, "Realloc failed on notPrinted\n");
                        exit(1);
                    }
                }
            }
            else {
                printPeriodMove(*possibleMove);

                if ((i + 1) < rankScore.numPossibleMoves) {
                    nextMove = rankScore.possibleMoves[i + 1];
                    if (nextMove.depthLevel != possibleMove->depthLevel) {
#ifdef DEBUG
                        printf("NextMove: %d CurrentMove: %d\n", nextMove.depthLevel, possibleMove->depthLevel);
#endif
                        int diff = possibleMove->depthLevel - nextMove.depthLevel;
                        while (diff-- > 0) {
                            toBePrinted = rankScore.possibleMoves[notPrinted[--numNotPrinted]];
                            printPeriodMove(toBePrinted);
                            notPrinted = realloc(notPrinted, (numNotPrinted + 1) * sizeof(int));
                            if (!notPrinted) {
                                fprintf(stderr, "Realloc failed on notPrinted\n");
                                exit(1);
                            }
                        }
                    }
                } else {
                    // end of all moves, empty notPrinted list
                    printNotPrintedList(notPrinted, numNotPrinted);
                }
            }
        }

        if (!rankScore.isVerbose && depth == 0) {
            exportMove(possibleMove->move, stdout);
            fprintf(stdout, " for %s: score %d\n", possibleMove->config.turn, possibleMove->score);
        }
    }
    free(notPrinted);
}

void printNotPrintedList(int *notPrinted, int numNotPrinted) {
#ifdef DEBUG
    int k;
    for (k = 0; k < numNotPrinted; k++) {
        fprintf(stdout, "%d ", notPrinted[k]);
    }
    fprintf(stdout, "\n");
#endif

    int g;
    PossibleMoves toBePrinted;
    for (g = numNotPrinted - 1; g >= 0; g--) {
        toBePrinted = rankScore.possibleMoves[notPrinted[g]];
        printPeriodMove(toBePrinted);
    }
}

void printQuestionMarkMove(PossibleMoves possibleMove) {
    if(possibleMove.depthLevel != 0) {
        fprintf(stdout, "%*c? ", 4 * possibleMove.depthLevel, ' ');
    }
    else {
        fprintf(stdout, "? ");
    }
    exportMove(possibleMove.move, stdout);
    fprintf(stdout, " for %s:\n", possibleMove.config.turn);
}

void printPeriodMove(PossibleMoves possibleMove) {
    if(possibleMove.depthLevel != 0) {
        fprintf(stdout, "%*c. ", 4 * possibleMove.depthLevel, ' ');
    }
    else {
        fprintf(stdout, ". ");
    }
    exportMove(possibleMove.move, stdout);
    fprintf(stdout, " for %s: score %d\n", possibleMove.config.turn, possibleMove.score);
}

void setNonDeepestScores() {
    int i;
    PossibleMoves *parentMove;
    PossibleMoves currentMove;

    for (i = rankScore.numPossibleMoves - 1; i >= 0; i--) {
        parentMove = &rankScore.possibleMoves[i];
        if (parentMove->score == 100) {
            if ((i + 1) < rankScore.numPossibleMoves) {
#ifdef DEBUG
                printf("Parent Move: ");
                exportMove(parentMove->move, stdout);
                printf("\n");
#endif
                int numScores = 0;
                int *subScores = malloc(sizeof(int));
                if (!subScores) {
                    fprintf(stderr, "Malloc failed on subScores.\n");
                    exit(1);
                }
                int j = i + 1;
                currentMove = rankScore.possibleMoves[j];
#ifdef DEBUG
                printf("Current Move: ");
                exportMove(currentMove.move, stdout);
                printf("\n");
#endif
                if (currentMove.depthLevel == parentMove->depthLevel) {
                    parentMove->score = calculateScore(parentMove);
#ifdef DEBUG
                    exportMove(parentMove->move, stdout);
                    fprintf(stdout, " for %s: score %d\n", parentMove->config.turn, parentMove->score);
#endif
                    continue;
                }
                while (currentMove.depthLevel > parentMove->depthLevel
                       && j < rankScore.numPossibleMoves
                       && currentMove.depthLevel != parentMove->depthLevel) {

                    if (currentMove.depthLevel == (parentMove->depthLevel + 1)) {
                        subScores[numScores++] = currentMove.score;
                        subScores = realloc(subScores, (numScores + 1) * sizeof(int));
                        if (!subScores) {
                            fprintf(stderr, "Realloc failed on subScores.\n");
                            exit(1);
                        }
                    }
                    currentMove = rankScore.possibleMoves[++j];
                }

#ifdef DEBUG
                int k;
                for (k = 0; k < numScores; k++) {
                    fprintf(stdout, "%d ", subScores[k]);
                }
                fprintf(stdout, "\n");
#endif
                parentMove->score = -maxScore(subScores, numScores);
                free(subScores);
            }
            else {
                parentMove->score = calculateScore(parentMove);
            }
#ifdef DEBUG
            exportMove(parentMove->move, stdout);
                fprintf(stdout, " for %s: score %d\n", parentMove->config.turn, parentMove->score);
#endif
        }
    }
}

void setDeepestScores() {
    PossibleMoves *currentMove;
    unsigned int i;
    for (i = 0; i < rankScore.numPossibleMoves; i++) {
        currentMove = &rankScore.possibleMoves[i];

        if (currentMove->depthLevel != rankScore.depth) {
            currentMove->score = 100;
            continue;
        }
        currentMove->score = calculateScore(currentMove);
    }
#ifdef DEBUG
    unsigned int g;
    for(g = 0; g < rankScore.numPossibleMoves; g++){
        exportMove(rankScore.possibleMoves[g].move, stdout);
        fprintf(stdout, " score: %d ", rankScore.possibleMoves[g].score);
        fprintf(stdout, "\n");
    }

#endif
}

int maxScore(int *scores, int numScores) {
    unsigned int i = 0;
    int max = scores[0];
    int score;
    for (i = 1; i < numScores; i++) {
        score = scores[i];
        if (score > max) {
            max = score;
        }
    }
    return max;
}

void addPossibleMove(Position origin, Position dest, int depth, bool isJump) {
#ifdef DEBUG
    char destCol = mapIntToLetter(dest.col);
    char originCol = mapIntToLetter(origin.col);
    char destRow = rowToBoardRow(dest.row);
    char originRow = rowToBoardRow(origin.row);
    printf("%c%d->%c%d for %s: score\n", originCol, originRow, destCol, destRow, setup.boardConfig.turn);
#endif
    PossibleMoves newMove;
    newMove.depthLevel = depth;
#ifdef DEBUG
    printf("Depth: %d\n", depth);
#endif
    copyBoardConfig(&setup.boardConfig, &newMove.config);
#ifdef DEBUG
    printf("RedPawns: %d RedKings: %d BlackPawns: %d BlackKings: %d\n", newMove.config.redPawns, newMove.config.redKings, newMove.config.blackPawns, newMove.config.blackKings);
#endif

    // will have to add additional logic to handle multiple jumps
    newMove.move.positions = malloc(2 * sizeof(Position));
    if (!newMove.move.positions) {
        fprintf(stderr, "Malloc failed on PossibleMoves->move.positions.\n");
        exit(1);
    }
    newMove.move.positions[0] = origin;
    newMove.move.positions[1] = dest;
    newMove.move.numPositions = 2;
    newMove.isJump = isJump;

    if(strcmp(setup.captureRule, "capture") == 0){
        rankScore.captureMoves = realloc(rankScore.captureMoves,
                                          (rankScore.numCaptureMoves + 1) * sizeof(PossibleMoves));
        if (!rankScore.captureMoves) {
            fprintf(stderr, "Realloc failed on rankScore.captureMoves");
            exit(1);
        }
        rankScore.captureMoves[rankScore.numCaptureMoves] = newMove;
        rankScore.numCaptureMoves++;
    }
    else {
        rankScore.possibleMoves = realloc(rankScore.possibleMoves,
                                          (rankScore.numPossibleMoves + 1) * sizeof(PossibleMoves));
        if (!rankScore.possibleMoves) {
            fprintf(stderr, "Realloc failed on rankScore.possibleMoves");
            exit(1);
        }
        rankScore.possibleMoves[rankScore.numPossibleMoves] = newMove;
        rankScore.numPossibleMoves++;
    }

    // switch turn
    alternateTurn(setup.boardConfig.turn);
}

int calculateScore(PossibleMoves *possibleMove) {
    BoardConfig board = possibleMove->config;
    bool isRedTurn = isRed(board.turn);
    int score;

#ifdef DEBUG
    printf("RedPawns: %d RedKings: %d BlackPawns: %d BlackKings: %d\n",
            possibleMove->config.redPawns, possibleMove->config.redKings,
            possibleMove->config.blackPawns, possibleMove->config.blackKings);
#endif
    score = (board.redPawns + (2 * board.redKings)) - (board.blackPawns + (2 * board.blackKings));
    if (!isRedTurn) {
        if (board.redKings == 0 && board.redPawns == 0) {
            return 99;
        }
        return -score;
    } else if (board.blackKings == 0 && board.blackPawns == 0) {
        return 99;
    }

#ifdef DEBUG
    printf("Score: %d\n", score);
#endif
    return score;
}