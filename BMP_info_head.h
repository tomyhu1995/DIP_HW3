#ifndef _BMP_info_head_
#define _BMP_info_head_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "definition.h"
#include "BMP_file_head.h"

#define FILE_HEAD_size 14

void INFOHEADER_to_byte(IMG img, char *buffer);

void INFOHEADER_size_Parser(char *buffer, INFOHEADER *output);
void INFOHEADER_widthAndheight_Parser(char *buffer, INFOHEADER *output);
void INFOHEADER_planes_Parser(char *buffer, INFOHEADER *output);
void INFOHEADER_bits_Parser(char *buffer, INFOHEADER *output);
void INFOHEADER_compression_Parser(char *buffer, INFOHEADER *output);
void INFOHEADER_imagesize_Parser(char *buffer, INFOHEADER *output);
void INFOHEADER_xyResolution_Parser(char *buffer, INFOHEADER *output);
void INFOHEADER_ncolours_Parser(char *buffer, INFOHEADER *output);
void INFOHEADER_importantcolours_Parser(char *buffer, INFOHEADER *output);
void INFOHEADER_Parser(IMG *img);

#endif