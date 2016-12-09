#ifndef _defi_
#define _defi_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEAD_SIZE 14
#define INFO_SIZE 40
#define BITS 8 //there are 8-bits per byte
#define INT_SIZE 4
#define WORD_SIZE 32

#ifndef _LOG_
#define _LOG_
	#define __FUNC__	(char *)__FUNCTION__// get function name
#endif

#ifndef _info_head_
#define _info_head_
	typedef struct { 
		unsigned int size;/* Info Header size in bytes */ 
		int width,height;/* Width and height of image */ 
		unsigned short int planes;/* Number of colour planes */ 
		unsigned short int bits; /* Bits per pixel */ 
		unsigned int compression; /* Compression type */ 
		unsigned int imagesize; /* Image size in bytes */ 
		int xresolution,yresolution; /* Pixels per meter */ 
		unsigned int ncolours; /* Number of colours */ 
		unsigned int importantcolours; /* Important colours */ 
	} INFOHEADER;
#endif

#ifndef _head_
#define _head_
	typedef struct { 
	/* type : Magic identifier, usually is BM(0x42,0x4d) */ 
	unsigned short int type; 
	unsigned int size;/* File size in bytes */ 
	unsigned short int reserved1, reserved2; /* reserved */ 
	unsigned int offset;/* Offset to image data, bytes */ 
	} FILEHEADER;
#endif

#ifndef _raw_
#define _raw_
	typedef struct {
	int x;
	int y;
	unsigned char *color;
	}RAW_DATA;
#endif
	
#ifndef _img_format_
#define _img_format_
	typedef struct _img_{
	FILE *file;
	FILEHEADER head;
	INFOHEADER info_head;
	unsigned char platte[1024];
	RAW_DATA *pixels;
	int num_of_pixel;
	}IMG;
#endif


#endif