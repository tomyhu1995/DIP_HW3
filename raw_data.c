#include "raw_data.h"

void COLOR_platte(IMG img, char *output){
	output = output + sizeof(INFOHEADER) + sizeof(FILEHEADER);
	for(int i = 0; i < img.head.offset - sizeof(INFOHEADER) + sizeof(FILEHEADER); i++){
		*output = (char)img.platte[i];
		//printf("platte = %d\n", *output);
		output = output + sizeof(char);
	}
}

void RAW_DATA_to_byte(IMG img, unsigned char *output){
	RAW_DATA *input = img.pixels;
	INFOHEADER info = img.info_head;
	FILEHEADER head = img.head;

	int counter = 0, shift;
	if(info.bits == 8){
		shift = (int)head.offset;
		COLOR_platte(img, output);
	}else{
		shift = (int)head.offset;
	}

	output = output + sizeof(unsigned char) * shift;

	int depth = info.bits / BITS;
	int RowSize = (int)floor(((info.bits * info.width) + 31 ) / 32) * 4;
	
	if(info.height > 0){
		for(int k = 0 ; k < abs(info.height) ; k ++){
			for(int j = 0 ; j < info.width ; j ++){
				if(depth == 1){
					*output = (unsigned char)img.pixels[counter].color[0];
					output = output + sizeof(unsigned char);
				}else{
					for(int i = 0 ; i < depth; i++){
						*output = img.pixels[counter].color[i];
						output = output + sizeof(unsigned char);
					}
				}
				
				counter++;
			}

			if(info.width % INT_SIZE != 0){
				//printf("Need padding\n");
				for(int k = 0; k < info.width % INT_SIZE ; k ++){
					*output = 0x00;//pad row size to multiple of 4 bytes
					output = output + sizeof(unsigned char);
				}
			}
		}
	}else{
		counter = info.width * abs(info.height) - 1;

		for(int k = 0 ; k < abs(info.height) ; k ++){
			for(int j = 0 ; j < info.width ; j ++){
				if(depth == 1){
					*output = (unsigned char)img.pixels[counter].color[0];
					output = output + sizeof(unsigned char);
				}else{
					for(int i = 0 ; i < depth; i++){
						*output = img.pixels[counter].color[i];
						output = output + sizeof(unsigned char);
					}
				}
				counter--;
			}

			if(info.width % INT_SIZE != 0){
				//printf("Need padding\n");
				for(int k = 0; k < info.width % INT_SIZE ; k ++){
					*output = 0x00;//pad row size to multiple of 4 bytes
					output = output + sizeof(unsigned char);
				}
			}
		}
	}
}

void RAW_DATA_to_byte_limit(IMG img, unsigned char *output, int left, int right, int top, int bottom){
	RAW_DATA *input = img.pixels;
	INFOHEADER info = img.info_head;
	FILEHEADER head = img.head;
	
	int counter = 0;
	unsigned char **tmp;
	tmp = (RAW_DATA **)malloc(sizeof(RAW_DATA *) * (img.info_head.height));

	for(int i = 0 ; i < img.info_head.width; i++){
		tmp[i] = (RAW_DATA *)malloc(sizeof(RAW_DATA) * (img.info_head.width));
	}

	for(int i = 0; i < img.info_head.height; i++){
		for(int j = 0; j < img.info_head.width; j++){
			tmp[i][j] = img.pixels[counter].color[0];
			counter++;
		}
	}

	int shift;
	if(info.bits == 8){
		shift = (int)head.offset;
		COLOR_platte(img, output);
	}else{
		shift = (int)head.offset;
	}

	output = output + sizeof(unsigned char) * shift;

	info.width = right - left +1;
	info.height = bottom - top + 1;

	int depth = info.bits / BITS;
	int RowSize = (int)floor(((info.bits * info.width) + 31 ) / 32) * 4;

	for(int k = top; k <= bottom; k ++){
		for(int j = left ; j <= right; j ++){
				*output = (unsigned char)tmp[k][j];
				output = output + sizeof(unsigned char);
		}

		if(info.width % INT_SIZE != 0){
			//printf("Need padding\n");
			for(int k = 0; k <  RowSize - info.width; k ++){
				*output = 0x00;//pad row size to multiple of 4 bytes
				output = output + sizeof(unsigned char);
			}
		}
	}
	
}

RAW_DATA *RAW_DATA_Parser(IMG *img){
	FILE *input = img->file;
	INFOHEADER info;
	FILEHEADER head;

	memcpy(&info, &(img->info_head), sizeof(INFOHEADER));
	memcpy(&head, &(img->head), sizeof(FILEHEADER));

	int i, j, m = 0, n = 0 , width_counter = 0;
	int RowSize = (int)floor(((info.bits * info.width) + 31 ) / 32) * 4;
	int PixelArraySize = RowSize * abs(info.height);

	RAW_DATA *output = (RAW_DATA *)malloc(sizeof(RAW_DATA) * info.width * abs(info.height));

	unsigned char *buffer = (char *)malloc(sizeof(char) * PixelArraySize);
	bzero(buffer, PixelArraySize);

	for(i = 0 ; i < info.width * abs(info.height) ; i++){
		output[i].color = (unsigned char *)malloc(sizeof(unsigned char) * (info.bits / BITS));
	}

	for(i = 0 ; i < PixelArraySize ; i++){
		fscanf(input, "%c", &buffer[i]);
	}
    
    if(info.height > 0){
    	for(i = info.height-1 ; i >= 0 ; i--){
			for(j = 0 ; j < RowSize ; j++){
				if(j < info.width*(info.bits/BITS)){
					output[m / (info.bits/BITS)].x = width_counter / (info.bits/BITS);
					output[m / (info.bits/BITS)].y = i;
					if(info.bits == 8){
						output[m / (info.bits/BITS)].color[0] = buffer[m];
					}else{
						output[m / (info.bits/BITS)].color[j % (info.bits / BITS)] = buffer[m];
					}
					//printf("--------------\n");
					//printf("(%d,%d)\n", output[m / (info.bits/BITS)].x, output[m / (info.bits/BITS)].y);
					//printf("--------------\n\n");
					width_counter++;
				}else{
					//do nothing
				}
				m++;
			}
			width_counter = 0;
		}
    }else{
    	for(i = 0 ; i < abs(info.height) ; i++){
			for(j = 0 ; j < RowSize ; j++){
				if(j < info.width*(info.bits/BITS)){
					output[n / (info.bits/BITS)].x = width_counter / (info.bits/BITS);
					output[n / (info.bits/BITS)].y = i;
					if(info.bits == 8){
						output[n / (info.bits/BITS)].color[0] = buffer[m];
					}else{
						output[n / (info.bits/BITS)].color[j % (info.bits / BITS)] = buffer[m];
					}
					
					//printf("--------------\n");
					//printf("(%d,%d)\n", output[m / (info.bits/BITS)].x, output[m / (info.bits/BITS)].y);
					//printf("--------------\n\n");
					width_counter++;
					n++;
				}else{
					//break;
				}
				m++;
			}
			width_counter = 0;
		}
    }
	
    free(buffer);

	img->num_of_pixel = info.width * info.height;
	return output;
}