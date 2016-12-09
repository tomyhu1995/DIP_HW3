#include "img_analysis.h"

void histogram(char *output_file_name, IMG data){
	int graylevel[256] = {0};
	

	FILE *fp = fopen(output_file_name, "w");
	if(fp == NULL){
		printf("Fail to open file %s\n", output_file_name);
	}

	for(int i = 0 ; i < data.num_of_pixel; i++){
		graylevel[data.pixels[i].color[0]] = graylevel[data.pixels[i].color[0]] + 1;
	}

	for(int i = 0 ; i < Eight_gray ; i++){
		//printf("level = %d\n", graylevel[i]);
		fprintf(fp, "%d\t%d\n",i, graylevel[i]);
	}

	fclose(fp);
}