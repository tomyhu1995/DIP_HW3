#include "BMP_info_head.h"

void INFOHEADER_size_to_byte(INFOHEADER input, char *buffer){
	memcpy(buffer + FILE_HEAD_size, &input.size, sizeof(input.size));
	//printf("%x %x %x %x", buffer[14], buffer[15], buffer[16], buffer[17]);
}

void INFOHEADER_widthAndheight_to_byte(INFOHEADER input, char *buffer){
	int shift = FILE_HEAD_size + sizeof(input.size);
	memcpy(buffer + shift, &input.width, sizeof(input.width));
	//printf("%x %x %x %x", buffer[18], buffer[19], buffer[20], buffer[21]);
	shift = shift + sizeof(input.width);
	memcpy(buffer + shift, &input.height, sizeof(input.height));
}

void INFOHEADER_planes_to_byte(INFOHEADER input, char *buffer){
	int shift = FILE_HEAD_size + sizeof(input.size) + sizeof(input.width) + sizeof(input.height);
	memcpy(buffer + shift, &input.planes, sizeof(input.planes));
	//printf("%x %x", buffer[26], buffer[27]);
}

void INFOHEADER_bits_to_byte(INFOHEADER input, char *buffer){
	int shift = FILE_HEAD_size + sizeof(input.size) + sizeof(input.width) + sizeof(input.height) + sizeof(input.planes);
	memcpy(buffer + shift, &input.bits, sizeof(input.bits));
}

void INFOHEADER_compression_to_byte(INFOHEADER input, char *buffer){
	int shift = FILE_HEAD_size + sizeof(input.size) + sizeof(input.width) + sizeof(input.height) + sizeof(input.planes) + sizeof(input.bits);
	memcpy(buffer + shift, &input.compression, sizeof(input.compression));
}

void INFOHEADER_imagesize_to_byte(INFOHEADER input, char *buffer){
	int shift = FILE_HEAD_size + sizeof(input.size) + sizeof(input.width) + sizeof(input.height) + sizeof(input.planes) + sizeof(input.bits) + sizeof(input.compression);
	memcpy(buffer + shift, &input.imagesize, sizeof(input.imagesize));
}

void INFOHEADER_xyResolution_to_byte(INFOHEADER input, char *buffer){
	int shift = FILE_HEAD_size + sizeof(input.size) + sizeof(input.width) + sizeof(input.height) + sizeof(input.planes) + sizeof(input.bits) + sizeof(input.compression) + sizeof(input.imagesize);
	memcpy(buffer + shift, &input.xresolution, sizeof(input.xresolution));
	shift = FILE_HEAD_size + sizeof(input.size) + sizeof(input.width) + sizeof(input.height) + sizeof(input.planes) + sizeof(input.bits) + sizeof(input.compression) + sizeof(input.imagesize) + sizeof(input.xresolution);
	memcpy(buffer + shift, &input.yresolution, sizeof(input.yresolution));
}

void INFOHEADER_ncolours_to_byte(INFOHEADER input, char *buffer){
	int shift = FILE_HEAD_size + sizeof(input.size) + sizeof(input.width) + sizeof(input.height) + sizeof(input.planes) + sizeof(input.bits) + sizeof(input.compression) + sizeof(input.imagesize) + sizeof(input.xresolution) + sizeof(input.yresolution);
	memcpy(buffer + shift, &input.ncolours, sizeof(input.ncolours));
}

void INFOHEADER_importantcolours_to_byte(INFOHEADER input, char *buffer){
	int shift = FILE_HEAD_size + sizeof(input.size) + sizeof(input.width) + sizeof(input.height) + sizeof(input.planes) + sizeof(input.bits) + sizeof(input.compression) + sizeof(input.imagesize) + sizeof(input.xresolution) + sizeof(input.yresolution) + sizeof(input.ncolours);
	memcpy(buffer + shift, &input.importantcolours, sizeof(input.importantcolours));
}

void INFOHEADER_to_byte(IMG img, char *buffer){
	INFOHEADER_size_to_byte(img.info_head, buffer);
	INFOHEADER_widthAndheight_to_byte(img.info_head, buffer);
	INFOHEADER_planes_to_byte(img.info_head, buffer);
	INFOHEADER_bits_to_byte(img.info_head, buffer);
	INFOHEADER_compression_to_byte(img.info_head, buffer);
	INFOHEADER_imagesize_to_byte(img.info_head, buffer);
	INFOHEADER_xyResolution_to_byte(img.info_head, buffer);
	INFOHEADER_ncolours_to_byte(img.info_head, buffer);
	INFOHEADER_importantcolours_to_byte(img.info_head, buffer);
}

void INFOHEADER_size_Parser(char *buffer, INFOHEADER *output){

	output->size = (buffer[3] << (BITS * 3)) + (buffer[2] << (BITS * 2)) + (buffer[1] << (BITS * 1)) + buffer[0];
}

void INFOHEADER_widthAndheight_Parser(char *buffer, INFOHEADER *output){
	output->width = (int)((buffer[7] << (BITS * 3)) + (buffer[6] << (BITS * 2)) + (buffer[5] << (BITS * 1)) + buffer[4]);
	output->height =(int)((buffer[11] << (BITS * 3)) + (buffer[10] << (BITS * 2)) + (buffer[9] << (BITS * 1)) + buffer[8]);
}

void INFOHEADER_planes_Parser(char *buffer, INFOHEADER *output){

	output->planes = (buffer[13] << BITS) + buffer[12];
}

void INFOHEADER_bits_Parser(char *buffer, INFOHEADER *output){

	output->bits = (buffer[15] << BITS) + buffer[14];
}

void INFOHEADER_compression_Parser(char *buffer, INFOHEADER *output){

	output->compression = (buffer[19] << (BITS * 3)) + (buffer[18] << (BITS * 2)) + (buffer[17] << (BITS * 1)) + buffer[16];
}

void INFOHEADER_imagesize_Parser(char *buffer, INFOHEADER *output){

	output->imagesize = (buffer[23] << (BITS * 3)) + (buffer[22] << (BITS * 2)) + (buffer[21] << (BITS * 1)) + buffer[20];
}

void INFOHEADER_xyResolution_Parser(char *buffer, INFOHEADER *output){

	output->xresolution = (buffer[27] << (BITS * 3)) + (buffer[26] << (BITS * 2)) + (buffer[25] << (BITS * 1)) + buffer[24];
	output->yresolution = (buffer[31] << (BITS * 3)) + (buffer[30] << (BITS * 2)) + (buffer[29] << (BITS * 1)) + buffer[28];
}

void INFOHEADER_ncolours_Parser(char *buffer, INFOHEADER *output){

	output->ncolours = (buffer[35] << (BITS * 3)) + (buffer[34] << (BITS * 2)) + (buffer[33] << (BITS * 1)) + buffer[32];
}

void INFOHEADER_importantcolours_Parser(char *buffer, INFOHEADER *output){

	output->importantcolours = (buffer[39] << (BITS * 3)) + (buffer[38] << (BITS * 2)) + (buffer[37] << (BITS * 1)) + buffer[36];
}

void INFOHEADER_Parser(IMG *img){
	FILE *input = img->file;
	int i = 0;
	char *buffer;
	char buffer_info_head_size[INT_SIZE];
	bzero(buffer_info_head_size, INT_SIZE);

	while(1){
		if(i < 4){
			fscanf(input, "%c", &buffer_info_head_size[i]);
			if(i == 3){
				INFOHEADER_size_Parser(buffer_info_head_size, &(img->info_head));
				if(img->info_head.size == 0){
					printf("Error to read info header, size = 0\n");
					break;
				}else if(img->info_head.size > 0xf0){
					printf("Error to read info header, size = %d\n", img->info_head.size);
					break;
				}else{
					buffer = (char *)malloc(sizeof(char) * (img->info_head.size));
					bzero(buffer, img->info_head.size);
				}
			}
		}else{
			fscanf(input, "%c", &buffer[i]);
		}

		if(i == (img->info_head.size -1)){
			break;
		}

		i++;
	}

	INFOHEADER_widthAndheight_Parser(buffer, &(img->info_head));
	INFOHEADER_planes_Parser(buffer, &(img->info_head));
	INFOHEADER_bits_Parser(buffer, &(img->info_head));
	INFOHEADER_compression_Parser(buffer, &(img->info_head));
	INFOHEADER_imagesize_Parser(buffer, &(img->info_head));
	INFOHEADER_xyResolution_Parser(buffer, &(img->info_head));
	INFOHEADER_ncolours_Parser(buffer, &(img->info_head));
	INFOHEADER_importantcolours_Parser(buffer, &(img->info_head));

}