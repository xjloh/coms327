#include "headers/config.h"
#include "headers/helpers.h"

Config setup;

int main() {

    evaluateConfigFile(stdin);
    setup.showMessages = true;
    printValidConfigInfo();
    deallocateChangeInfo();
    return 0;
}
