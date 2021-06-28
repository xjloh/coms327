#include "headers/config.h"
#include "headers/helpers.h"
#include "headers/rank.h"

#define MULTI_JUMPS
//#define CAPTURE
#define DEBUG

RankScore rankScore;

void isImplemented();

int main(int argc, char *argv[]) {
    unsigned int i;
    char *configFileName = NULL;
    FILE *configFile;
    bool isVerbose = false;
    int depth = 0;
    bool depthFound = false;
    char *strDepth = NULL;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            isVerbose = true;
        } else if (strcmp(argv[i], "-d") == 0) {
            if (depthFound) {
                fprintf(stderr, "Ignoring additional -d switch\n");
                ++i;
                continue;
            }
            depthFound = true;
            strDepth = argv[++i];
            if (!strDepth) {
                fprintf(stderr, "D value following `-d` was not found.");
                exit(1);
            } else {
                char *ptr;
                int ret = strtol(strDepth, &ptr, 10);
                if (strcmp(ptr, "") != 0) {
                    fprintf(stderr, "Invalid int for D: %s", strDepth);
                    exit(1);
                }
                if (ret < 0) {
                    ret = 0;
                }
                depth = ret;
            }
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

    rankScore.depth = depth;
    rankScore.isVerbose = isVerbose;

    evaluateConfigFile(configFile);
    setup.showMessages = true;
    isImplemented();
    // exchange files with no moves given
    // could also ignore moves & print warning/error
    if (setup.numMoves != 0) {
        fprintf(stderr, "Moves detected. Invalid input file.");
        exit(1);
    }
    calculateRank();
    return 0;
}

/**
 * Error messages for unimplemented features
 */
void isImplemented() {
#ifdef MULTI_JUMPS
    if (strcmp(setup.jumpRule, "multiple jumps") == 0) {
        fprintf(stderr, "Sorry, `multiple jumps` has not been implemented");
        exit(1);
    }
#endif
#ifdef CAPTURE
    if (strcmp(setup.captureRule, "capture") == 0) {
        fprintf(stderr, "Sorry, `capture` has not been implemented");
        exit(1);
    }
#endif
}
