#include "helpers.h"
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
	for(int h = 0; h < height; ++h){
		for(int w= 0; w < width; ++w){
			int avg = (int)round((image[h][w].rgbtRed + image[h][w].rgbtGreen + image[h][w].rgbtBlue)/3.0);
			image[h][w].rgbtRed = image[h][w].rgbtGreen = image[h][w].rgbtBlue = avg;
		}
	}
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
	for(int h = 0; h < height; ++h){
		for(int w= 0; w < width; ++w){

			int originalRed = image[h][w].rgbtRed;
			int originalGreen = image[h][w].rgbtGreen;
			int originalBlue = image[h][w].rgbtBlue;

			int sepiaRed = (int)round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
			int sepiaGreen	= (int) round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
			int sepiaBlue = (int)round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);

			image[h][w].rgbtRed = sepiaRed <= 255 ? sepiaRed : 255;
			image[h][w].rgbtGreen = sepiaGreen <= 255 ? sepiaGreen : 255;
			image[h][w].rgbtBlue = sepiaBlue <= 255 ? sepiaBlue: 255;

		}
	}
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
	for(int h = 0; h < height; ++h){
		for(int w= 0; w < width/2; ++w){
			int temp;

			temp = image[h][w].rgbtRed;
			image[h][w].rgbtRed = image[h][width-1 - w].rgbtRed;
			image[h][width-1 - w].rgbtRed = temp;


			temp = image[h][w].rgbtGreen;
			image[h][w].rgbtGreen = image[h][width-1 - w].rgbtGreen;
			image[h][width-1 - w].rgbtGreen = temp;

			temp = image[h][w].rgbtBlue;
			image[h][w].rgbtBlue = image[h][width-1 - w].rgbtBlue;
			image[h][width-1 - w].rgbtBlue = temp;

		}
	}
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
	RGBTRIPLE ** original_image = malloc(sizeof(RGBTRIPLE*)*height);
	for(int i = 0; i < height; ++i){
		original_image[i] = malloc(sizeof(RGBTRIPLE)*width);

	}

	for(int h = 0; h < height; ++h){
		for(int w= 0; w < width; ++w){
			original_image[h][w].rgbtRed = image[h][w].rgbtRed;
			original_image[h][w].rgbtGreen = image[h][w].rgbtGreen;
			original_image[h][w].rgbtBlue = image[h][w].rgbtBlue;
		}
	}

	for(int h = 0; h < height; ++h){
		for(int w= 0; w < width; ++w){

			int startH = h-1;
			int endH = h+1;
			int startW = w-1;
			int endW = w+1;

			if(h == 0)
				startH = 0;
			if(h == height-1)
				endH = h;
			if(w == 0)
				startW = 0;
			if(w == width-1)
				endW = w;

			int total = (endH - startH + 1)*(endW - startW + 1);
			double newRed = 0, newGreen = 0, newBlue = 0;

			for(int i = startH; i <= endH; ++i){
				for(int j = startW; j <= endW; ++j){
					newRed += original_image[i][j].rgbtRed;
					newGreen += original_image[i][j].rgbtGreen;
					newBlue += original_image[i][j].rgbtBlue;
				}
			}
			image[h][w].rgbtRed = (int)round(newRed/total);
			image[h][w].rgbtGreen = (int)round(newGreen/total);
			image[h][w].rgbtBlue = (int)round(newBlue/total);

		}
	}
	for(int i = 0; i < height; ++i)
		free(original_image[i]);

	free(original_image);
    return;
}

void edges(int height, int width, RGBTRIPLE image[height][width])
{
	RGBTRIPLE ** original_image = malloc(sizeof(RGBTRIPLE*)*height);
	for(int i = 0; i < height; ++i){
		original_image[i] = malloc(sizeof(RGBTRIPLE)*width);

	}

	for(int h = 0; h < height; ++h){
		for(int w= 0; w < width; ++w){
			original_image[h][w].rgbtRed = image[h][w].rgbtRed;
			original_image[h][w].rgbtGreen = image[h][w].rgbtGreen;
			original_image[h][w].rgbtBlue = image[h][w].rgbtBlue;
		}
	}

	for(int h = 0; h < height; ++h){
		for(int w= 0; w < width; ++w){

			int startH = h-1;
			int endH = h+1;
			int startW = w-1;
			int endW = w+1;

			if(h == 0)
				startH = 0;
			if(h == height-1)
				endH = h;
			if(w == 0)
				startW = 0;
			if(w == width-1)
				endW = w;

			struct {
				int red, green, blue;
			}Gx, Gy;

			int x_mat[3][3] = {
				{-1,0,1},
				{-2,0,2},
				{-1,0,1}
			};

			int y_mat[3][3] = {
				{-1,-2,-1},
				{0,0,0},
				{1,2,1}
			};
			Gx.red = Gx.green = Gx.blue = 0;
			Gy.red = Gy.green = Gy.blue = 0;

			for(int i = startH; i <= endH; ++i){
				for(int j = startW; j <= endW; ++j){
					Gx.red += original_image[i][j].rgbtRed * x_mat[1+i-h][1+j-w] ;
					Gx.green += original_image[i][j].rgbtGreen * x_mat[1+i-h][1+j-w];
					Gx.blue += original_image[i][j].rgbtBlue * x_mat[1+i-h][1+j-w];

					Gy.red += original_image[i][j].rgbtRed * y_mat[1+i-h][1+j-w] ;
					Gy.green += original_image[i][j].rgbtGreen * y_mat[1+i-h][1+j-w] ;
					Gy.blue += original_image[i][j].rgbtBlue * y_mat[1+i-h][1+j-w] ;
				}
			}


			int newRed= (int)round(sqrt(Gx.red*Gx.red + Gy.red*Gy.red));
			int newGreen= (int)round(sqrt(Gx.green*Gx.green + Gy.green*Gy.green));
			int newBlue = (int)round(sqrt(Gx.blue*Gx.blue + Gy.blue*Gy.blue));

			if(newRed > 255) image[h][w].rgbtRed = 255;
			else image[h][w].rgbtRed = newRed;

			if(newGreen > 255) image[h][w].rgbtGreen = 255;
            else image[h][w].rgbtGreen = newGreen;


			if(newBlue > 255) image[h][w].rgbtBlue = 255;
            else image[h][w].rgbtBlue = newBlue;

		}
	}
	for(int i = 0; i < height; ++i)
		free(original_image[i]);

	free(original_image);
    return;
}
