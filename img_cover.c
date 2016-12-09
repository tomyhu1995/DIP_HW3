#include "img_cover.h"

void Two_gray_level(IMG img, IMG *output){
	char grayscale;
	memcpy(output, &img, sizeof(img));


	for(int i = 0; i < img.num_of_pixel; i++){
		grayscale = img.pixels[i].color[0] * 0.114 + img.pixels[i].color[1] * 0.587 + img.pixels[i].color[2] * 0.299;
		output->pixels[i].color[0] = grayscale;
		output->pixels[i].color[1] = grayscale;
		output->pixels[i].color[2] = grayscale;
	}
}

void img_cover(IMG *background, IMG *cover, int cover_x, int cover_y){
	if(background == NULL || cover == NULL){
		printf("Invalid input image\n");
		return;
	}

	if(background->info_head.width < cover->info_head.width || background->info_head.height < cover->info_head.height){
		printf("Background is too small to cover it\n");
		return;
	}

	int width = cover->info_head.width;
	int height = cover->info_head.height;

	int x_start = cover_x - width /2;
	int y_start = cover_y + height / 2;
	int x_end = cover_x + width / 2;
	int y_end = cover_y - height / 2;

	int cover_counter = 0;
	//printf("x_start = %d, x_end = %d, y_start = %d, y_end = %d\n", x_start, x_end, y_start, y_end);
	for(int i = 0; i < background->num_of_pixel ; i++){
		//printf("No.%d, x = %d, y = %d\n", i, background->pixels[i].x, background->pixels[i].y);
		if(background->pixels[i].x >= x_start && background->pixels[i].x < x_end){
			if(background->pixels[i].y <= y_start && background->pixels[i].y > y_end){
				if(cover_counter < cover->num_of_pixel){
					//printf("Background No.%d, x = %d, y = %d\t", i, background->pixels[i].x, background->pixels[i].y);
					//printf("cover No.%d, x = %d, y = %d\n", cover_counter, cover->pixels[cover_counter].x, cover->pixels[cover_counter].y);
					for(int n = 0; n < background->info_head.bits / BITS; n++){
						background->pixels[i].color[n] = cover->pixels[cover_counter].color[n];
					}
					cover_counter++;
				}else{
					break;
				}
			}
			
		}
		
	}

}