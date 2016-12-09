#include "BMP_file_head.h"

void FILEHEADER_type_to_byte(FILEHEADER input, char *buffer){
	memcpy(buffer, &input.type, sizeof(input.type));
}

void FILEHEADER_size_to_byte(FILEHEADER input, char *buffer){
	memcpy(buffer+sizeof(input.type), &input.size, sizeof(input.size));
	//printf("%x %x %x %x\n", buffer[2], buffer[3], buffer[4], buffer[5]);
}

void FILEHEADER_reserved_to_byte(FILEHEADER input, char *buffer){
	memcpy(buffer + sizeof(input.type) + sizeof(input.size), &input.reserved1, sizeof(input.reserved1));
	memcpy(buffer + sizeof(input.type) + sizeof(input.size) + sizeof(input.reserved1), &input.reserved2, sizeof(input.reserved2));
	//printf("%x %x\n", buffer[6], buffer[7]);
}

void FILEHEADER_offset_to_byte(FILEHEADER input, char *buffer){
	memcpy(buffer + sizeof(input.type) + sizeof(input.size) + sizeof(input.reserved1) + sizeof(input.reserved2), &input.offset, sizeof(input.offset));
	//printf("%x %x %x %x\n", buffer[10], buffer[11], buffer[12], buffer[13]);
}

void FILEHEADER_to_byte(IMG img, char *buffer){
	FILEHEADER_type_to_byte(img.head, buffer);
	FILEHEADER_size_to_byte(img.head, buffer);
	FILEHEADER_reserved_to_byte(img.head, buffer);
	FILEHEADER_offset_to_byte(img.head, buffer);
}

void FILEHEADER_type_Parser(char *buffer, FILEHEADER *output){

	output->type = (buffer[1] << BITS) + buffer[0];
}

void FILEHEADER_size_Parser(char *buffer, FILEHEADER *output){

	output->size = (buffer[5] << (BITS * 3)) + (buffer[4] << (BITS * 2)) + (buffer[3] << (BITS * 1)) + buffer[2];
}

void FILEHEADER_reserved_Parser(char *buffer, FILEHEADER *output){
	output->reserved1 = (buffer[7] << BITS) + buffer[6];
	output->reserved2 = (buffer[9] << BITS) + buffer[8];
}

void FILEHEADER_offset_Parser(char *buffer, FILEHEADER *output){
	
	output->offset = (buffer[13] << (BITS * 3)) + (buffer[12] << (BITS * 2)) + (buffer[11] << (BITS)) + buffer[10];
}

void FILEHEADER_Parser(IMG *img){
	FILE *input = img->file;
	int i = 0;
	char buffer[HEAD_SIZE];
	bzero(buffer, HEAD_SIZE);

	for(i = 0 ; i < HEAD_SIZE ; i ++){
		fscanf(input, "%c", &buffer[i]);
	}

	FILEHEADER_type_Parser(buffer, &(img->head));
	FILEHEADER_size_Parser(buffer, &(img->head));
	FILEHEADER_reserved_Parser(buffer, &(img->head));
	FILEHEADER_offset_Parser(buffer, &(img->head));

	//printf("type = %d\n", output->type);
	//printf("size = %d\n", output->size);
	//printf("reserved = %d %d\n", output->reserved1, output->reserved2);
	//printf("offset = %d\n", output->offset);
}