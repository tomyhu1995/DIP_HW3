#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "EasyBMP.h"
using namespace std;

extern "C"{
	#include "definition.h"
	#include "BMP_file_head.h"
	#include "BMP_info_head.h"
	#include "raw_data.h"
	#include "img_cover.h"
	#include "img_analysis.h"
	#include "filter.h"
	#include "LINE_detect.h"
	#include "IMG_CREATE.h"
}

#define tmp_file "tmp.bmp"

void PRINT_IMG_INFO(IMG img){
	printf("----------------Image header----------------\n");
	printf("type = %d\n", img.head.type);
	printf("Total size = %d\n", img.head.size);
	printf("reserved 1 = %d, reserved 2 = %d\n", img.head.reserved1, img.head.reserved2);
	printf("offset = %d\n", img.head.offset);
	printf("------------------------------------------\n");

	printf("****************Image info******************\n");
	printf("info header size = %d\n", img.info_head.size);
	printf("width = %d, height = %d\n", img.info_head.width, img.info_head.height);
	printf("planes = %d\n", img.info_head.planes);
	printf("bits = %d\n", img.info_head.bits);
	printf("compression = %d\n", img.info_head.compression);
	printf("imagesize = %d\n", img.info_head.imagesize);
	printf("xresolution = %d, yresolution = %d\n", img.info_head.xresolution, img.info_head.yresolution);
	printf("number of color = %d\n", img.info_head.ncolours);
	printf("imporant color = %d\n", img.info_head.importantcolours );
	printf("********************************************\n\n");
}

void PRINT_COLOR_TABLE(IMG *img){
	int i;

	for(i = 0; i < 1024; i++){
		printf("%x\n",i , img->platte[i]);
	}
}

void PRINT_IMG_PIXEL(IMG *img){
	int i, j; 

	for(i = 0; i < img->info_head.width; i++){
		for(j = 0; j < img->info_head.height; j++){
			printf("%x ", img->pixels[i].color);
		}
		printf("\n");
	}
}

void IMG_read(char *filename, IMG *img){
	img->file = fopen(filename, "r");

	if(img->file == NULL){
		printf("Fail to open file , file name = %s\n", filename);
		return;
	}

	FILEHEADER_Parser(img);
	INFOHEADER_Parser(img);
	PRINT_IMG_INFO(*img);

	if(img->info_head.bits == 8){
		int i;
		for(i = 0 ; i < 1024 ; i++){
			fscanf(img->file, "%c", &(img->platte[i]));
		}
	}
	img->pixels = RAW_DATA_Parser(img);
	
	fclose(img->file);
}

void IMG_copy(IMG *img, IMG *img_out){
	IMG img_tmp;
	char system_call[50] = {0};

	//IMG_create(tmp_file, *img);
	//IMG_read(tmp_file, &img_tmp);

	memcpy(&(img_out->head), &(img->head), sizeof(FILEHEADER));
	memcpy(&(img_out->info_head), &(img->info_head), sizeof(INFOHEADER));
	
	if(img->info_head.bits == 8){
		memcpy(img_out->platte, img->platte, 1024);
	}

	img_out->pixels = (RAW_DATA *)malloc(sizeof(RAW_DATA) * img->info_head.width * img->info_head.height);
	for(int i = 0; i < img->info_head.width * img->info_head.height; i++){
		img_out->pixels[i].color = (unsigned char *)malloc(sizeof(unsigned char) * (img->info_head.bits/BITS));
		//memset(img_out->pixels[i].color, 0, img_tmp.info_head.bits / BITS);
	}
	
	//snprintf(system_call, 50, "rm %s", tmp_file);
	//system(system_call);
}

void IMG_READ(char *filename, IMG *output_img){
	IMG tmp;
	char command[50] = {0};
	IMG_read(filename, &tmp);
	IMG_create(tmp_file, tmp);
	IMG_read(tmp_file, output_img);
	snprintf(command, 50, "rm %s", tmp_file);
	system(command);
}

void Binarization(IMG *img){
	int i;
	for(i = 0; i < img->info_head.width * img->info_head.height; i++){
		if(img->pixels[i].color[0] >= 100){
			img->pixels[i].color[0] = 255;
		}else{
			img->pixels[i].color[0] = 0;
		}
	}
}

void background_removal(IMG *img){
	unsigned char **tmp;
	int *cumulative_vertical;
	int *cumulative_horizontal;
	int counter = 0;
	int left, right, top, bottom;

	tmp = (unsigned char **)malloc(sizeof(unsigned char *) * (img->info_head.height));
	cumulative_vertical = (int *)malloc(sizeof(int) * img->info_head.width);
	memset(cumulative_vertical, 0, sizeof(int) * img->info_head.width);
	cumulative_horizontal = (int *)malloc(sizeof(int) * img->info_head.height);
	memset(cumulative_horizontal, 0, sizeof(int) * img->info_head.height);

	for(int i = 0 ; i < img->info_head.width; i++){
		tmp[i] = (unsigned char *)malloc(sizeof(unsigned char) * (img->info_head.width));
	}
	for(int i = 0; i < img->info_head.height; i++){
		for(int j = 0; j < img->info_head.width; j++){
			tmp[i][j] = img->pixels[counter].color[0];
			counter++;
		}
	}

	for(int j = 0; j < img->info_head.width; j++){
		for(int i = 0; i < img->info_head.height; i++){
			cumulative_vertical[j] += tmp[i][j];
		}
	}

	for(int j = 0; j < img->info_head.height; j++){
		for(int i = 0; i < img->info_head.width; i++){
			cumulative_horizontal[j] += tmp[j][i];
		}
	}

	for(int j = 0; j < img->info_head.width; j++){
		if(cumulative_vertical[j+1] < cumulative_vertical[j]){
			left = j;
			break;
		}
	}

	for(int j = img->info_head.width-1; j > 0; j--){
		if(cumulative_vertical[j-1] < cumulative_vertical[j]){
			right = j;
			break;
		}
	}

	for(int j = 0; j < img->info_head.height; j++){
		if(cumulative_horizontal[j+1] < cumulative_horizontal[j]){
			top = j;
			break;
		}
	}

	for(int j = img->info_head.height-1; j > 0; j--){
		if(cumulative_horizontal[j-1] < cumulative_horizontal[j]){
			bottom = j;
			break;
		}
	}

	printf("left = %d\t", left);
	printf("right = %d\t", right);
	printf("top = %d\t", top);
	printf("bottom = %d\n", bottom);

	free(cumulative_horizontal);
	free(cumulative_vertical);

	BMP Background;
	Background.ReadFromFile("image/f0542_07.bmp");

	BMP Output;
	Output.SetSize( right - left + 1, bottom - top +1);
	 //Output.SetBitDepth( 24 );
	Output.SetBitDepth( 8 );
	RangedPixelToPixelCopy( Background, left, right, bottom, top, Output, 0,0 );	
							 
	cout << "writing 8bpp ... " << endl;					
	Output.WriteToFile( "HW3_1.bmp" );


}

int main(int argc, char const *argv[])
{
	IMG img;
	IMG_READ("image/f0542_07.bmp", &img);
	Binarization(&img);
	background_removal(&img);

	return 0;
}