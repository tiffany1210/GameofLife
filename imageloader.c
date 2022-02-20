/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				Tiffany Kim
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) {
	FILE *fp;
	fp = fopen(filename, "r");
	char buf[3];
	uint32_t cols, rows;
	uint8_t scale;

	Image *img = (Image *) malloc(sizeof(Image));
	if (img == NULL) {
		fprintf(stderr, "Unable to allocate memory\n");
        exit(-1);
    }

    fscanf(fp, "%s", buf);

    fscanf(fp,  "%" SCNu32 "%" SCNu32, &cols, &rows);

    img->rows = rows;
    img->cols = cols;

    fscanf(fp, "%hhu",  &scale);

    Color **imgptr = (Color **) malloc(cols * rows * sizeof(Color *));
    if (imgptr == NULL) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(-1);
    }


    for (int i = 0; i< (cols * rows); i++) {
    	Color *newcolor = (Color *) malloc(sizeof(Color));
    	if (newcolor == NULL) {
			fprintf(stderr, "Unable to allocate memory\n");
        	exit(-1);
    	}
    	uint8_t r, g, b;
    	fscanf(fp, "%hhu %hhu %hhu", &r, &g, &b);
    	newcolor->R = r;
    	newcolor->G = g;
    	newcolor->B = b;
    	imgptr[i] = newcolor;
    }
    img->image = imgptr;
	fclose(fp);
	return img;
}




//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	printf("%s\n", "P3");
	printf("%d %d\n", image->cols, image->rows);
	printf("%d\n", 255);

	for (int i = 0; i < image->rows; i++) {
		int j = 0;
		while (j < image->cols-1) {
			Color *clr = (image->image)[(image->cols)*i + j];
			printf("%3d %3d %3d   ", clr->R, clr->G, clr->B);
			j++;
		}
		Color *clr = (image->image)[(image->cols)*i + j];
		printf("%3d %3d %3d\n", clr->R, clr->G, clr->B);
	}		
}

//Frees an image
void freeImage(Image *image)
{
	for (int i = 0; i < (image->cols * image->rows); i++) {
		free((image->image)[i]);
	}
	free((image->image));
	free(image);
}