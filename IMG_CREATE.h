#ifndef _IMG_CREATE_
#define _IMG_CREATE_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "definition.h"

int IMG_create(char *filename, IMG img);
int IMG_create_limit(char *filename, IMG img, int left, int right, int top, int bottom);

#endif