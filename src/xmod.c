#include "change.h"
#include "regfile.h"
#include "xmod_info.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static clock_t startClock;
static int logFile;

int main(int argc, char * argv[]) {

    //Start the clock:
    startClock = clock();
    return 0;
}