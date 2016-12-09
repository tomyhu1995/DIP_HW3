#ifndef _img_cover_
#define _img_cover_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "definition.h"

void Two_gray_level(IMG img, IMG *output);
void img_cover(IMG *backgournd, IMG *cover, int cover_x, int cover_y);

#endif