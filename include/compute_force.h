#include <stdio.h>
#include <string.h>
#include <math.h>
#include "datastructure.h"
#include "utilities.h"

static void velverlet(mdsys_t *sys);
static void force(mdsys_t *sys);