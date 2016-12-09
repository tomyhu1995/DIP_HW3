#ifndef _FILTER_
#define _FILTER_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "definition.h"

void Gradient_filter(IMG *input, IMG *output, int color_mask);
void Gaussian_S_domain_filter(IMG *input, IMG *output, int color_mask);

#endif