#include "LINE_detect.h"

#define feature_point 100
#define background 0
#define PI 3.14159265358979

void Hough_Transform(IMG *input, IMG *output){
	float sin_tab[180], cos_tab[180];
	float theta;
	float rmax=sqrt((float)input->info_head.height*(float)input->info_head.height+(float)input->info_head.width*(float)input->info_head.width);

	unsigned char **tmp;
	unsigned char **TMP;
	int counter = 0;
	

	tmp = (RAW_DATA **)malloc(sizeof(RAW_DATA *) * (input->info_head.height));
	TMP = (unsigned char **)malloc(sizeof(unsigned char *) * abs(2*rmax + 1));

	for(int i = 0 ; i < input->info_head.height; i++){
		tmp[i] = (RAW_DATA *)malloc(sizeof(RAW_DATA) * (input->info_head.width));
	}

	for(int i = 0; i < abs(2*rmax + 1); i++){
		TMP[i] = (unsigned char *)malloc(sizeof(unsigned char) * 180);
	}

	for(int i = 0; i < abs(2*rmax + 1); i++){
		for(int j = 0; j < 180; j++){
			TMP[i][j] = 0;
		}
	}

	for(int i = 0; i < input->info_head.height; i++){
		for(int j = 0; j < input->info_head.width; j++){
			tmp[i][j] = input->pixels[counter].color[0];
			counter++;
		}
	}


    for(int angle=0;angle<180;angle++)
    {
         theta=angle*PI/180.0;
         cos_tab[angle]=cos(theta);
         sin_tab[angle]=sin(theta);
    } 

    float r;
    int nr,angle;

    for(int i=0; i< input->info_head.height; i++){
    	for(int j=0; j< input->info_head.width; j++){
	      
	      if(tmp[j][i] >= feature_point)
	      {
	           for(angle=0; angle< 180; angle++)  // for each theta
	           {
	               r=i*sin_tab[angle]+j*cos_tab[angle];// compute r
	               nr = (int)(r+rmax);
	               TMP[nr][angle]++;           // accumulation
	           }
	      }
    	}
    }

    for(int i = 0; i < abs(2*rmax + 1); i++){
    	for(int j = 0 ; j < 180; j++){
    		printf("%d\n", TMP[i][j]);	
    	}
    }
}