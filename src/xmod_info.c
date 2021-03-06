#include "xmod_info.h"

int fillXmodInfo(XmodInfo * xi, int argc, char * argv[]) {
    fillXmodFlags(&(xi->flags), argc, argv);  // Preencher options
    // Preencher modo
    // Preencher filename;
}

int fillXmodFlags(XmodFlags * xf, int argc, char * argv[]) {
    // iterar argv
}

bool isOctal(char * mode) {}

int convertToOctal(char * mode, mode_t * octalMode) {}

bool compareModes(mode_t mode1, mode_t mode2) {}
