#ifndef C_INPUT_H
#define C_INPUT_H
#include <stdio.h>
#include <string.h>
#include "datastructure.h"
#include "utilities.h"

int input(FILE *fp, char *line, char *restfile, char *trajfile, char *ergfile, mdsys_t *sys, int nprint, int BLEN);

#endif
