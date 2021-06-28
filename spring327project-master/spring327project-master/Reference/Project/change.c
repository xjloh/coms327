#include "headers/config.h"
#include "headers/helpers.h"
#include "headers/moves.h"

// #define MULTI_JUMPS
// #define CAPTURE

MoveFlags mFlags;

void isImplemented();

int main(int argc, char *argv[]) {

    unsigned i;
    bool restrictMoves = false;
    char *eFileName = NULL;
    char *hFileName = NULL;
    char *configFileName = NULL;
    char *strMove = NULL;

    Files eFiles;
    eFiles.files = malloc(sizeof(FILE **));
    Files hFiles;
    hFiles.files = malloc(sizeof(FILE **));
    int numHFiles = 0;
    int numEFiles = 0;

    unsigned int movesToExecute;
    FILE *configFile;
    FILE *eFile;
    FILE *hFile;

    for (i = 1; i < argc; i++) {
#ifdef DEBUG
        printf("%s\n", argv[i]);
#endif
        if (strcmp(argv[i], "-e") == 0) {
            eFileName = argv[++i];
#ifdef DEBUG
            printf("%s\n", eFileName);
#endif
            eFile = fopen(eFileName, "w");
            if (!eFile) {
                fprintf(stderr, "'%s' can't be opened\n", eFileName);
                exit(1);
            }
            eFiles.files = realloc(eFiles.files, (numEFiles + 1) * sizeof(FILE **));
            eFiles.files[numEFiles] = eFile;
            numEFiles++;
#ifdef DEBUG
            fprintf(eFile, "cheese");
            fclose(mFlags.eFileName);
#endif
        } else if (strcmp(argv[i], "-h") == 0) {
            hFileName = argv[++i];
#ifdef DEBUG
            printf("%s\n", hFileName);
#endif
            hFile = fopen(hFileName, "w");
            if (!hFile) {
                fprintf(stderr, "'%s' can't be opened\n", hFileName);
                exit(1);
            }
            hFiles.files = realloc(hFiles.files, (numHFiles + 1) * sizeof(FILE **));
            hFiles.files[numHFiles] = hFile;
            numHFiles++;
#ifdef DEBUG
            fprintf(mFlags.hFileName, "cheese");
            fclose(mFlags.hFileName);
#endif
        } else if (strcmp(argv[i], "-m") == 0) {
            if (restrictMoves) {
                fprintf(stderr, "Ignoring additional -m switch\n");
                ++i;
                continue;
            }
            restrictMoves = true;
            strMove = argv[++i];
            if (!strMove) {
                fprintf(stderr, "N value following `-m` was not found.");
                exit(1);
            } else {
                char *ptr;
                int ret = strtol(strMove, &ptr, 10);
                if (strcmp(ptr, "") != 0) {
                    fprintf(stderr, "Invalid int for N: %s", strMove);
                    exit(1);
                }
                if (ret < 0) {
                    ret = 0;
                }
                movesToExecute = ret;
            }
#ifdef DEBUG
            printf("%d\n", movesToExecute);
#endif
            mFlags.movesToExecute = movesToExecute;
        } else {
            if (configFileName) {
                fprintf(stderr, "Only one input file allowed\n");
                exit(1);
            }
            configFileName = argv[i];
            configFile = fopen(configFileName, "r");
            if (!configFile) {
                fprintf(stderr, "'%s' can't be opened\n", configFileName);
                exit(1);
            }
        }
    }
    if (!configFileName) {
        configFile = stdin;
    }
    if (!restrictMoves) {
        mFlags.movesToExecute = -1;
    }
    eFiles.numFiles = numEFiles;
    hFiles.numFiles = numHFiles;

    mFlags.eFiles = eFiles;
    mFlags.hFiles = hFiles;

    evaluateConfigFile(configFile);
    setup.showMessages = true;
    isImplemented();
    exportFiles();
    deallocateChangeInfo();
    return 0;
}

/**
 * Error messages for unimplemented features
 */
void isImplemented() {
#ifdef MULTI_JUMPS
    if (strcmp(setup.jumpRule, "multiple jumps") == 0){
        fprintf(stderr, "Sorry, `multiple jumps` has not been implemented");
        exit(1);
    }
#endif
#ifdef CAPTURE
    if (strcmp(setup.captureRule, "capture") == 0){
        fprintf(stderr, "Sorry, `capture` has not been implemented");
        exit(1);
    }
#endif
}