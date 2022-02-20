/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				Tiffany Kim
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	Color *clr = (image->image)[image->cols * row + col];
	int blue = clr->B;
	blue &= 1;
	Color *newclr = (Color *) malloc(sizeof(Color));
	if (newclr == NULL) {
		fprintf(stderr, "Unable to allocate memory\n");
        exit(-1);
    }
	if (blue == 1) {
		newclr->R = 255;
		newclr->G = 255;
		newclr->B = 255;
	} else {
		newclr->R = 0;
		newclr->G = 0;
		newclr->B = 0;
	}
	return newclr;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	Image *newimg = (Image *) malloc(sizeof(Image));
	if (newimg == NULL) {
		fprintf(stderr, "Unable to allocate memory\n");
        exit(-1);
    }
    newimg->rows = image->rows;
    newimg->cols = image->cols;

    Color **imgptr = (Color **) malloc(image->cols * image->rows * sizeof(Color *));
    if (imgptr == NULL) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(-1);
    }
	for (int i = 0; i < image->rows; i++) {
		for (int j = 0; j < image->cols; j++) {
			Color *clr = evaluateOnePixel(image, i, j);
			imgptr[(image->cols)*i + j] = clr;
		}
	}
	newimg->image = imgptr;
	return newimg;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	char *filename = argv[1];
	if (!filename) {
		exit(-1);
	}
	Image *img = readData(filename);
	if (!img) {
		fprintf(stderr, "Unable to read file '%s'\n", filename);
		exit(-1);
	}
	Image *resultimg = steganography(img);
	if (!resultimg) {
		fprintf(stderr, "Unable to read file '%s'\n", filename);
		exit(-1);
	}
	writeData(resultimg);
	freeImage(resultimg);
	freeImage(img);
}
