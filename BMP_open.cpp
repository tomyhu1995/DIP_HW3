#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>

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

#define file_path "image/f0542_07.bmp"
//#define file_path "image/s0554_02.bmp"
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

	//free(cumulative_horizontal);
	//free(cumulative_vertical);

	BMP Background;
	Background.ReadFromFile(file_path);

	BMP Output;
	Output.SetSize( right - left + 1, bottom - top +1);
	Output.SetBitDepth( Background.TellBitDepth());
	if(Output.TellBitDepth() < 16){
		CreateGrayscaleColorTable(Output);
	}
	RangedPixelToPixelCopy( Background, left, right, bottom, top, Output, 0, 0);
							 
	cout << "writing HW3_1.bmp ... " << endl;					
	Output.WriteToFile( "HW3_1.bmp" );
	//Background.WriteToFile("HW3_1.bmp");

}

unsigned char MaxNeighbor(unsigned char *input, int width, int height){
	unsigned char MAX = 0;

	for(int j = 0; j < height * width; j++){
		if(input[j] > MAX){
				MAX = input[j];
		}
	}
	return MAX;
}

unsigned char MinNeighbor(unsigned char *input, int width, int height){
	unsigned char Min = 255;

	for(int i = 0 ; i < width * height; i++){
		if(input[i] < Min){
			Min = input[i];
		}
	}
	
	return Min;
}

void Erosion(unsigned char **input, unsigned char **output, int width, int height){
	unsigned char sample_mask[3][3] = { 
										{1,1,1},
										{1,1,1},
										{1,1,1}};

	unsigned char tmp_neighbor[9] = {0};

	int row_count =0, col_count=0;
	int counter = 0;
	unsigned char new_value;
	for(int i = 1; i < width - 1 ; i++){
		for(int j = 1; j < height - 1; j++){
			/*
			 * matching
			 */
			for(int column = i-1; column <= i+1; column++){//column
				for(int row = j-1; row <= j+1; row++){//row
					tmp_neighbor[counter] = input[row][column] * sample_mask[row_count][col_count];
					counter++;
					row_count++;
				}

				row_count = 0;
				col_count++;
			}
			new_value = MaxNeighbor(tmp_neighbor, 3, 3);

			for(int column = i-1; column <= i+1; column++){//column
				for(int row = j-1; row <= j+1; row++){//row
					output[row][column] = new_value;
				}
			}

			counter = 0;
			row_count = 0;
			col_count = 0;
		}
	}

}

void Dilation(unsigned char **input, unsigned char **output, int width, int height){
	unsigned char sample_mask[3][3] = { 
										{1,1,1},
										{1,1,1},
										{1,1,1}};

	unsigned char tmp_neighbor[9] = {0};
	unsigned char new_value;
	int row_count =0, col_count=0;
	int counter = 0;
	for(int i = 1; i < width - 1 ; i++){
		for(int j = 1; j < height - 1; j++){
			/*
			 * matching
			 */
			for(int column = i-1; column <= i+1; column++){//column
				for(int row = j-1; row <= j+1; row++){//row
					tmp_neighbor[counter] = input[row][column] * sample_mask[row_count][col_count];
					row_count++;
					counter++;
				}
				row_count = 0;
				col_count++;
			}
			
			new_value = MinNeighbor(tmp_neighbor, 3, 3);

			for(int column = i-1; column <= i+1; column++){//column
				for(int row = j-1; row <= j+1; row++){//row
					output[row][column] = new_value;
				}
			}

			counter = 0;
			row_count = 0;
			col_count = 0;
		}
	}

}

void opening(BMP input, char *filename){
	BMP output_img;
	output_img.SetSize(input.TellWidth(), input.TellHeight());
	output_img.SetBitDepth(8);
	CreateGrayscaleColorTable(output_img);

	unsigned char **tmp;
	unsigned char **output;
	unsigned char **input_pixel;
	int counter = 0;
	input_pixel = (unsigned char **)malloc(sizeof(unsigned char *) * (input.TellHeight()));
	tmp = (unsigned char **)malloc(sizeof(unsigned char *) * (input.TellHeight()));
	output = (unsigned char **)malloc(sizeof(unsigned char *) * (input.TellHeight()));

	for(int i = 0 ; i < input.TellHeight(); i++){
		tmp[i] = (unsigned char *)malloc(sizeof(unsigned char) * (input.TellWidth()));
		output[i] = (unsigned char *)malloc(sizeof(unsigned char *) * (input.TellWidth()));
		input_pixel[i] = (unsigned char *)malloc(sizeof(unsigned char *) * (input.TellWidth()));
	}

	for(int i = 0; i < input.TellHeight(); i++){
		for(int j = 0; j < input.TellWidth(); j++){
			input_pixel[i][j] = input.GetPixel(j,i).Red;
			tmp[i][j] = input.GetPixel(j,i).Red;
			//output[i][j] = input.GetPixel(j,i).Red;
			counter++;
		}
	}
	
	Erosion(input_pixel, tmp, input.TellWidth(), input.TellHeight());
	Dilation(tmp, output, input.TellWidth(), input.TellHeight());
	

	for(int i = 0; i < input.TellHeight(); i++){
		for(int j = 0; j < input.TellWidth(); j++){
			RGBApixel NewPixel;
			NewPixel.Alpha = input.GetPixel(j,i).Alpha;
			NewPixel.Blue = output[i][j];
			NewPixel.Green = output[i][j];
			NewPixel.Red = output[i][j];
			output_img.SetPixel( j, i, NewPixel);
		}
	}

	for(int i = 0 ; i < input.TellHeight(); i++){
		//free(tmp[i]);
		//free(output[i]);
		//free(input_pixel[i]);
	}

	//free(tmp);
	//free(output);
	//free(input_pixel);

	cout << "writing opening.bmp ... " << endl;					
	output_img.WriteToFile(filename);

}

void Closing(BMP input, char *filename){
	BMP output_img;
	output_img.SetSize(input.TellWidth(), input.TellHeight());
	output_img.SetBitDepth(8);
	CreateGrayscaleColorTable(output_img);

	unsigned char **tmp;
	unsigned char **output;
	unsigned char **input_pixel;
	int counter = 0;
	input_pixel = (unsigned char **)malloc(sizeof(unsigned char *) * (input.TellHeight()));
	tmp = (unsigned char **)malloc(sizeof(unsigned char *) * (input.TellHeight()));
	output = (unsigned char **)malloc(sizeof(unsigned char *) * (input.TellHeight()));

	for(int i = 0 ; i < input.TellHeight(); i++){
		tmp[i] = (unsigned char *)malloc(sizeof(unsigned char) * (input.TellWidth()));
		output[i] = (unsigned char *)malloc(sizeof(unsigned char *) * (input.TellWidth()));
		input_pixel[i] = (unsigned char *)malloc(sizeof(unsigned char *) * (input.TellWidth()));
	}

	for(int i = 0; i < input.TellHeight(); i++){
		for(int j = 0; j < input.TellWidth(); j++){
			input_pixel[i][j] = input.GetPixel(j,i).Red;
			tmp[i][j] = input.GetPixel(j,i).Red;
			output[i][j] = input.GetPixel(j,i).Red;
			counter++;
		}
	}
	
	Dilation(input_pixel, tmp, input.TellWidth(), input.TellHeight());
	Erosion(tmp, output, input.TellWidth(), input.TellHeight());	

	for(int i = 0; i < input.TellHeight(); i++){
		for(int j = 0; j < input.TellWidth(); j++){
			RGBApixel NewPixel;
			NewPixel.Alpha = input.GetPixel(j,i).Alpha;
			NewPixel.Blue = output[i][j];
			NewPixel.Green = output[i][j];
			NewPixel.Red = output[i][j];
			output_img.SetPixel( j, i, NewPixel);
		}
	}

	for(int i = 0 ; i < input.TellHeight(); i++){
		//free(tmp[i]);
		//free(output[i]);
		//free(input_pixel[i]);
	}

	//free(tmp);
	//free(output);
	//free(input_pixel);

	cout << "writing closing.bmp ... " << endl;					
	output_img.WriteToFile(filename);

}

void Sobel_x(unsigned char **input, unsigned char **output, int width, int height){
	int sample_mask[3][3] = { 
										{-1,0,1},
										{-2,0,2},
										{-1,0,1}};
	const int min = -1020;
	const int max = 1020;
	int new_value = 0;

	int row_count =0, col_count=0;
	for(int i = 1; i < width - 1 ; i++){
		for(int j = 1; j < height - 1; j++){
			/*
			 * matching
			 */
			for(int column = i-1; column <= i+1; column++){//column
				for(int row = j-1; row <= j+1; row++){//row
					new_value += input[row][column] * sample_mask[row_count][col_count];
					row_count++;
				}
				row_count = 0;
				col_count++;
			}
			
			output[j][i] = (unsigned char)new_value * 255 / (max - min);

			new_value = 0;
			row_count = 0;
			col_count = 0;
		}
	}
}

void Sobel_y(unsigned char **input, unsigned char **output, int width, int height){
	int sample_mask[3][3] = { 
										{-1,-2,-1},
										{0,0,0},
										{1,2,1}};
	const int min = -1020;
	const int max = 1020;
	int new_value = 0;

	int row_count =0, col_count=0;
	for(int i = 1; i < width - 1 ; i++){
		for(int j = 1; j < height - 1; j++){
			/*
			 * matching
			 */
			for(int column = i-1; column <= i+1; column++){//column
				for(int row = j-1; row <= j+1; row++){//row
					new_value += input[row][column] * sample_mask[row_count][col_count];
					row_count++;
				}
				row_count = 0;
				col_count++;
			}
			
			output[j][i] = (unsigned char)new_value * 255 / (max - min);

			new_value = 0;
			row_count = 0;
			col_count = 0;
		}
	}
}

void avg_filter(int **input, int **output, int width, int height){
	int sample_mask[15][15] = {1};

	int new_value = 0;

	int row_count =0, col_count=0;
	for(int i = 7; i < width - 7 ; i++){
		for(int j = 7; j < height - 7; j++){
			/*
			 * matching
			 */
			for(int column = i-7; column <= i+7; column++){//column
				for(int row = j-7; row <= j+7; row++){//row
					new_value += input[row][column] * sample_mask[row_count][col_count];
					row_count++;
				}
				row_count = 0;
				col_count++;
			}
			
			output[j][i] = new_value / 15;

			new_value = 0;
			row_count = 0;
			col_count = 0;
		}
	}

}

void multiply(unsigned char **A, unsigned char **B, int **output, unsigned char coefficient ,int width, int height){
	for(int i = 0; i < width; i++){
		for(int j = 0 ; j < height; j++){
			output[j][i] = (int)coefficient * A[j][i] * B[j][i];
		}
	}
}

void V_y_Calculate(unsigned char **G_x, unsigned char **G_y, int **V_y, int width, int height){
	for(int i = 0 ; i < height; i++){
		for (int j = 0; j < width; j++){
			V_y[i][j] = (int)(G_x[i][j] * G_x[i][j] - G_y[i][j] * G_y[i][j]);
		}
	}
}

void thida_d_Cal(int **phi_x, int **phi_y, double **thida_d, int width, int height){
	printf("width = %d, height = %d\n", width, height);
	for(int i = 0 ; i < height; i++){
		for (int j = 0; j < width; j++){
			double phi_X = (double)phi_x[i][j];
			double phi_Y = (double)phi_y[i][j];
			printf("phi_x[%d][%d] = %f, phi_y[%d][%d] = %f\t", i,j, phi_X, i,j,phi_Y);
			double tmp = phi_Y/phi_X;
			thida_d[i][j] = 0.5 * atan(tmp);
			printf("thida_d_Cal = %f\n", thida_d[i][j]);
		}
	}
}

void vector_cal(double **thida_d, double **vec, int width, int height){
	double Scale = 8;
	for(int i = 0 ; i <= (height-16) / 16; i++){
		for (int j = 0; j <= (width - 16) / 16; j++){
			vec[i][j] = Scale * cos(thida_d[i*16][j*16]);
		}
	}
}

void sample_matrix_x(int width, int height, int **output){
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			output[i][j] = (j+1)*16;
			//printf("%d ", output[i][j]);
		}
		//printf("\n");
	}
}

void sample_matrix_y(int width, int height, int **output){
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			output[i][j] = (i+1)*16;
			//printf("%d ", output[i][j]);
		}
		//printf("\n");
	}
}

void Orientation(BMP input, char *filename){
	BMP output_img;
	output_img.SetSize(input.TellWidth(), input.TellHeight());
	output_img.SetBitDepth(8);
	CreateGrayscaleColorTable(output_img);

	unsigned char **G_x;
	unsigned char **G_y;
	int **V_x;
	int **V_y;
	int **phi_x;
	int **phi_y;
	double **thida_d;
	double **u;
	double **v;
	int **x;
	int **y;
	unsigned char **input_pixel;
	int counter = 0;

	input_pixel = (unsigned char **)malloc(sizeof(unsigned char *) * (input.TellHeight()));
	G_x = (unsigned char **)malloc(sizeof(unsigned char *) * (input.TellHeight()));
	G_y = (unsigned char **)malloc(sizeof(unsigned char *) * (input.TellHeight()));
	V_x = (int **)malloc(sizeof(int *) * (input.TellHeight()));
	V_y = (int **)malloc(sizeof(int *) * (input.TellHeight()));
	phi_x = (int **)malloc(sizeof(int *) * (input.TellHeight()));
	phi_y = (int **)malloc(sizeof(int *) * (input.TellHeight()));
	thida_d = (double **)malloc(sizeof(double *) * (input.TellHeight()));
	u = (double **)malloc(sizeof(double *) * (input.TellHeight()));
	v = (double **)malloc(sizeof(double *) * (input.TellHeight()));
	x = (int **)malloc(sizeof(int *) * (input.TellHeight()));
	y = (int **)malloc(sizeof(int *) * (input.TellHeight()));

	for(int i = 0 ; i < input.TellHeight(); i++){
		G_y[i] = (unsigned char *)malloc(sizeof(unsigned char) * (input.TellWidth()));
		G_x[i] = (unsigned char *)malloc(sizeof(unsigned char *) * (input.TellWidth()));
		input_pixel[i] = (unsigned char *)malloc(sizeof(unsigned char *) * (input.TellWidth()));
		V_x[i] = (int *)malloc(sizeof(int *) * (input.TellWidth()));
		V_y[i] = (int *)malloc(sizeof(int *) * (input.TellWidth()));
		phi_x[i] = (int *)malloc(sizeof(int *) * (input.TellWidth()));
		phi_y[i] = (int *)malloc(sizeof(int *) * (input.TellWidth()));
		thida_d[i] = (double *)malloc(sizeof(double *) * (input.TellWidth()));
		u[i] = (double *)malloc(sizeof(double *) * (input.TellWidth()));
		v[i] = (double *)malloc(sizeof(double *) * (input.TellWidth()));
		x[i] = (int *)malloc(sizeof(int *) * (input.TellWidth()));
		y[i] = (int *)malloc(sizeof(int *) * (input.TellWidth()));
	}

	for(int i = 0; i < input.TellHeight(); i++){
		for(int j = 0; j < input.TellWidth(); j++){
			input_pixel[i][j] = input.GetPixel(j,i).Red;
			G_x[i][j] = input.GetPixel(j,i).Red;
			G_y[i][j] = input.GetPixel(j,i).Red;
			V_x[i][j] = input.GetPixel(j,i).Red;
			V_y[i][j] = input.GetPixel(j,i).Red;
			phi_x[i][j] = input.GetPixel(j,i).Red;
			phi_y[i][j] = input.GetPixel(j,i).Red;
			counter++;
		}
	}
	//printf("Sobel\n");
	Sobel_x(input_pixel, G_x, input.TellWidth(), input.TellHeight());
	Sobel_y(input_pixel, G_y, input.TellWidth(), input.TellHeight());
	//printf("multiply\n");
	multiply(G_x, G_y, V_x, 2, input.TellWidth(), input.TellHeight());
	//printf("V_y_Calculate\n");
	V_y_Calculate(G_x, G_y, V_y, input.TellWidth(), input.TellHeight());
	//printf("avg_filter\n");
	avg_filter(V_x, phi_x, input.TellWidth(), input.TellHeight());
	avg_filter(V_y, phi_y, input.TellWidth(), input.TellHeight());
	//printf("thida_d_Cal\n");
	thida_d_Cal(phi_x, phi_y, thida_d, input.TellWidth(), input.TellHeight());
	//printf("vector_cal\n");
	vector_cal(thida_d, u, input.TellWidth(), input.TellHeight());
	vector_cal(thida_d, v, input.TellWidth(), input.TellHeight());
	//printf("sample_matrix\n");
	sample_matrix_x(input.TellWidth(), input.TellHeight(), x);
	sample_matrix_y(input.TellWidth(), input.TellHeight(), y);

	/*printf("u\n");
	for(int i = 0; i < input.TellHeight(); i++){
		for(int j = 0; j < input.TellWidth(); j++){
			printf("%f ", u[i][j]);
		}
		printf("\n");
	}
	printf("\n");*/

	/*printf("v\n");
	for(int i = 0; i < input.TellHeight(); i++){
		for(int j = 0; j < input.TellWidth(); j++){
			printf("%f ", v[i][j]);
		}
		printf("\n");
	}
	printf("\n");*/

	/*printf("x\n");
	for(int i = 0; i < input.TellHeight(); i++){
		for(int j = 0; j < input.TellWidth(); j++){
			printf("%d ", x[i][j]);
		}
		printf("\n");
	}
	printf("\n");*/

	/*printf("y\n");
	for(int i = 0; i < input.TellHeight(); i++){
		for(int j = 0; j < input.TellWidth(); j++){
			printf("%d ", y[i][j]);
		}
		printf("\n");
	}
	printf("\n");*/


	/*for(int i = 0; i < input.TellHeight(); i++){
		for(int j = 0; j < input.TellWidth(); j++){
			RGBApixel NewPixel;
			NewPixel.Alpha = input.GetPixel(j,i).Alpha;
			NewPixel.Blue = output[i][j];
			NewPixel.Green = output[i][j];
			NewPixel.Red = output[i][j];
			output_img.SetPixel( j, i, NewPixel);
		}
	}*/

	//cout << "writing testing.bmp ... " << endl;					
	//output_img.WriteToFile(filename);

}

void scanAllPixel(BMP input){
	for(int i = 0; i < input.TellWidth() ;i++){
		for(int j = 0; j < input.TellHeight(); j++){
			RGBApixel pixel;
			pixel = input.GetPixel(i, j);

			if(pixel.Red != pixel.Green || pixel.Green != pixel.Blue){
				printf("Error at (%d,%d), R = %d, G = %d, B = %d, Alpha = %d\n", i,j, pixel.Red, pixel.Green, pixel.Blue, pixel.Alpha);
			}
		}

	}
}

int main(int argc, char const *argv[])
{
	char opt;
	IMG img;

	printf("background_removal, please enter 'a'\n");
	printf("opening, please enter 'b'\n");
	printf("closing, please enter 'c'\n");
	printf("Calculate orientation field 'd' : ");
	scanf("%c", &opt);

	switch(opt){
		case 'a':{
			IMG_READ(file_path, &img);
			Binarization(&img);
			background_removal(&img);
			break;
		}
		case 'b':{
			IMG_READ(file_path, &img);
			Binarization(&img);
			background_removal(&img);
			BMP input_1;
			input_1.ReadFromFile("HW3_1.bmp");
			opening(input_1, "opening.bmp");
			break;
		}
		case 'c':{
			IMG_READ(file_path, &img);
			Binarization(&img);
			background_removal(&img);
			BMP input_2;
			input_2.ReadFromFile("HW3_1.bmp");
			Closing(input_2, "closing.bmp");
			break;
		}
		case 'd':{
			IMG_READ(file_path, &img);
			Binarization(&img);
			background_removal(&img);
			BMP input_3;
			input_3.ReadFromFile("HW3_1.bmp");
			Orientation(input_3, "testing.bmp");
			break;
		}
		default:{
			printf("Invalid input, please try again\n");
			break;
		}
	}



	return 0;
}