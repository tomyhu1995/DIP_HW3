#ifndef _RAW_DATA_
#define _RAW_DATA_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "definition.h"

void RAW_DATA_to_byte(IMG img, unsigned char *output);
void RAW_DATA_to_byte_limit(IMG img, unsigned char *output, int left, int right, int top, int bottom);
RAW_DATA *RAW_DATA_Parser(IMG *img);

#endif