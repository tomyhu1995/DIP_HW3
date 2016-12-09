#include "filter.h"

void Gradient_filter(IMG *input, IMG *output, int color_mask){
	//printf("Enter [%s]\n", __FUNC__);
	unsigned char **tmp;
	int counter = 0;
	float **TMP;

	tmp = (RAW_DATA **)malloc(sizeof(RAW_DATA *) * (input->info_head.height));
	TMP = (float **)malloc(sizeof(float *) * (input->info_head.height));

	for(int i = 0 ; i < input->info_head.height; i++){
		tmp[i] = (RAW_DATA *)malloc(sizeof(RAW_DATA) * (input->info_head.width));
		TMP[i] = (float *)malloc(sizeof(float) * (input->info_head.width));
	}

	for(int i = 0; i < input->info_head.height; i++){
		for(int j = 0; j < input->info_head.width; j++){
			tmp[i][j] = input->pixels[counter].color[color_mask];
			counter++;
		}
	}

	for(int i = 0; i < input->info_head.height; i++){
		for(int j = 0; j < input->info_head.width; j++){
			if(i-1 < 0 || j-1 < 0 || i+1 == input->info_head.height || j+1 == input->info_head.width){
				//do nothing
			}else{
				//TMP[i][j] = (float)(tmp[i-1][j] * 1 + tmp[i][j-1] * 1 + tmp[i][j] * -4 + tmp[i][j+1] * 1 + tmp[i+1][j] * 1);
				TMP[i][j] = (float)(tmp[i][j-1] * 1 + tmp[i][j] * -2 + tmp[i][j+1] * 1);
				//printf("%d\n", tmp[i][j]);
			}
			
		}
	}

	counter = 0;
	for(int i = 0; i < input->info_head.height; i++){
		for(int j = 0; j < input->info_head.width; j++){
				output->pixels[counter].color[color_mask] = abs(TMP[i][j]);
				counter++;
		}
	}

	for(int i = 0; i < input->info_head.height; i++){
		free(tmp[i]);
		free(TMP[i]);
	}

	//printf("Exit [%s]\n", __FUNC__);
}

void Gaussian_S_domain_filter(IMG *input, IMG *output, int color_mask){
	printf("Enter [%s]\n", __FUNC__);
	unsigned char **tmp;
	int counter = 0;

	tmp = (RAW_DATA **)malloc(sizeof(RAW_DATA *) * (input->info_head.height+2));

	for(int i = 0 ; i < input->info_head.width + 2; i++){
		tmp[i] = (RAW_DATA *)malloc(sizeof(RAW_DATA) * (input->info_head.width + 2));
	}

	for(int i = 1; i < input->info_head.height+1; i++){
		for(int j = 1; j < input->info_head.width+1; j++){
			tmp[i][j] = input->pixels[counter].color[color_mask];
			counter++;
		}
	}

	for(int i = 0; i < input->info_head.width + 2; i++){
		if(i == 0){
			//printf("1. %d %d\n", 0, i);
			tmp[0][i] = input->pixels[0].color[color_mask];

			//printf("2. %d %d\n", input->info_head.height+1, i);
			tmp[input->info_head.height+1][i] = input->pixels[input->info_head.height * (input->info_head.height - 1)].color[color_mask];
		
		}else if(i == input->info_head.width + 1){
			//printf("3. %d %d\n",0, 1);
			tmp[0][i] = input->pixels[input->info_head.width - 1].color[color_mask];

			//printf("4. %d %d\n", input->info_head.height+1, i);
			tmp[input->info_head.height+1][i] = input->pixels[input->info_head.height*input->info_head.width - 1].color[color_mask];
		
		}else{
			//printf("5. %d %d\n", 0, i);
			tmp[0][i] = input->pixels[i-1].color[color_mask];

			//printf("6. %d %d\n", input->info_head.height+1, i);
			tmp[input->info_head.height+1][i] = input->pixels[input->info_head.width * (input->info_head.height - 1) + i - 1].color[color_mask];
		}
	}

	for(int i = 1; i < input->info_head.height; i++){
		tmp[i][0] = input->pixels[(i-1) * input->info_head.width].color[color_mask];
		tmp[i][input->info_head.width + 1] = input->pixels[(i-1) * input->info_head.width + input->info_head.width - 1].color[color_mask];
	}

	counter = 0;
	//for(int i = 1; i < input->info_head.height+1; i++){
	//	for(int j = 1; j < input->info_head.width+1; j++){
			//printf("%d %d\n", tmp[i][j], input->pixels[counter].color[0]);
	//		counter++;
	//	}
	//}

	for(int i = 1; i < input->info_head.height+1; i++){
		for(int j = 1; j < input->info_head.width+1; j++){
			float TMP = (float)1/9 * (tmp[i-1][j-1] * 1 + tmp[i-1][j] * 1 + tmp[i-1][j+1] * 1 + tmp[i][j-1] * 1 + tmp[i][j] * 1 + tmp[i][j+1] * 1 + tmp[i+1][j-1] * 1 + tmp[i+1][j] * 1 + tmp[i+1][j+1] * 1);
			//tmp[i][j] = (unsigned char) 1/16 * (tmp[i-1][j-1] * 1 + tmp[i-1][j] * 2 + tmp[i-1][j+1] * 1 + tmp[i][j-1] * 2 + tmp[i][j] * 4 + tmp[i][j+1] * 2 + tmp[i+1][j-1] * 1 + tmp[i+1][j] * 2 + tmp[i+1][j+1] * 1);
			//printf("%f\n", TMP);
			tmp[i][j] = (unsigned char)TMP;
			//printf("%d\n", tmp[i][j]);
		}
	}

	counter = 0;

	for(int i = 1; i < input->info_head.height+1; i++){
		for(int j = 1; j < input->info_head.width+1; j++){
			for(int k = 0; k < input->info_head.bits / BITS; k++){
				output->pixels[counter].color[k] = tmp[i][j];
			}
			
			counter++;
		}
	}

	//for(int i = 1; i < input->info_head.width+1; i++){
	//	printf("%d\n", tmp[130][i]);

	//}
	for(int i = 0; i < input->info_head.height; i++){
		free(tmp[i]);
	}


	printf("Exit [%s]\n", __FUNC__);
}