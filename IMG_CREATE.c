#include "IMG_CREATE.h"

int IMG_create(char *filename, IMG img){
	FILE *output = fopen(filename, "w");
	unsigned char *buffer = (unsigned char *)malloc(sizeof(char) * (img.head.size+1));
	bzero(buffer, img.head.size);

	if(output == NULL){
		printf("Fail to open file, file name= %s\n", filename);
		return -1;
	}

	FILEHEADER_to_byte(img, buffer); 
	INFOHEADER_to_byte(img, buffer);
	RAW_DATA_to_byte(img, buffer);

	for(int i = 0; i < img.head.size ; i++){
		//fprintf(output, "%c", buffer[i]);
		fputc(buffer[i], output);
	}
	

	/*printf("\n\n\n");
	for(int i = 0; i < img.head.size; i++){
		if(i == img.head.offset){
			printf("\nRaw data:\n");
		}
		printf("%x ", buffer[i]);
	}*/
	free(buffer);
	fclose(output);
}

int IMG_create_limit(char *filename, IMG img, int left, int right, int top, int bottom){
	int tmp_w = img.info_head.width;
	int tmp_h = img.info_head.height;

	FILE *output = fopen(filename, "w");
	unsigned char *buffer = (unsigned char *)malloc(sizeof(char) * (img.head.size+1));
	memset(buffer, 0, (img.head.size+1));
	//bzero(buffer, img.head.size);

	if(output == NULL){
		printf("Fail to open file, file name= %s\n", filename);
		return -1;
	}
	
	img.info_head.width = right - left+1;
	img.info_head.height = bottom - top+1;
	img.info_head.imagesize = img.info_head.width * img.info_head.height;
	img.head.size = img.info_head.imagesize + img.head.offset;
	FILEHEADER_to_byte(img, buffer); 
	INFOHEADER_to_byte(img, buffer);
	
	img.info_head.width = tmp_w;
	img.info_head.height = tmp_h;
	RAW_DATA_to_byte_limit(img, buffer, left, right, top, bottom);

	for(int i = 0; i < img.head.size ; i++){
		//fprintf(output, "%c", buffer[i]);
		fputc(buffer[i], output);
	}
	

	/*printf("\n\n\n");
	for(int i = 0; i < img.head.size; i++){
		if(i == img.head.offset){
			printf("\nRaw data:\n");
		}
		printf("%x ", buffer[i]);
	}*/
	free(buffer);
	fclose(output);
}