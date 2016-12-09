#ifndef _BMP_FILE_HEAD_
#define _BMP_FILE_HEAD_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "definition.h"

void FILEHEADER_type_to_byte(FILEHEADER input, char *buffer);
void FILEHEADER_size_to_byte(FILEHEADER input, char *buffer);
void FILEHEADER_reserved_to_byte(FILEHEADER input, char *buffer);
void FILEHEADER_offset_to_byte(FILEHEADER input, char *buffer);
void FILEHEADER_to_byte(IMG img, char *buffer);

void FILEHEADER_type_Parser(char *buffer, FILEHEADER *output);
void FILEHEADER_size_Parser(char *buffer, FILEHEADER *output);
void FILEHEADER_reserved_Parser(char *buffer, FILEHEADER *output);
void FILEHEADER_offset_Parser(char *buffer, FILEHEADER *output);
void FILEHEADER_Parser(IMG *img);

#endif
