#ifndef C_INPUT_H
#define C_INPUT_H
#include <stdio.h>
#include <string.h>
#include "datastructure.h"
#include "utilities.h"

extern int input(FILE *fp, char *line, char *restfile, char *trajfile, char *ergfile, mdsys_t *sys, int nprint);
#endif