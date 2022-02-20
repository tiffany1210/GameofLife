/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				Tiffany Kim
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//return n mod (modnum)
int mod(int n, int modnum) {
	if (n == -1) {
		return modnum - 1;
	} else if (n == modnum) {
		return 0;
	}
}

int checkat(uint8_t rgbbits, int n) {
	int mask = 1 << n;
	rgbbits &= mask;
	return rgbbits >> n;
}

//return 1 if we have to turn on the bit of the *cell's specified rgb
//otherwise return 0
int turnbit(Color *cell, Color **neighborlist, int n, char rgb, uint32_t rule) {
	int count = 0;
	int cellbit = 0;
	if (rgb == 'R') {
		cellbit = checkat(cell->R, n);
		for (int i = 0; i < 8; i++) {
			count += checkat(neighborlist[i]->R, n);
		}
	} else if (rgb == 'G') {
		cellbit = checkat(cell->G, n);
		for (int i = 0; i < 8; i++) {
			count += checkat(neighborlist[i]->G, n);
		}
	} else if (rgb == 'B') {
		cellbit = checkat(cell->B, n);
		for (int i = 0; i < 8; i++) {
			count += checkat(neighborlist[i]->B, n);
		}
	} else {
		printf("%s\n", "Wrong RGB type");
		exit(-1);
	}
	int checkbit = 0;
	if (cellbit == 0) {
		int n = 1 << count;
		checkbit = (rule & n) >> count;
	} else if (cellbit == 1) {
		int n = 1 << (9 + count);
		checkbit = (rule & n) >> (count + 9);
	}
	return checkbit;
}

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	int numcols = image->cols;
	int numrows = image->rows;
	Color *cellclr = (image->image)[numcols * row + col];
	Color *n1 = (image->image)[numcols * mod(row - 1, numrows) + mod(col - 1, numcols)];
	Color *n2 = (image->image)[numcols * mod(row - 1, numrows) + col];
	Color *n3 = (image->image)[numcols * mod(row - 1, numrows) + mod(col + 1, numcols)];
	Color *n4 = (image->image)[numcols * row + mod(col - 1, numcols)];
	Color *n5 = (image->image)[numcols * row + mod(col + 1, numcols)];
	Color *n6 = (image->image)[numcols * mod(row + 1, numrows) + mod(col - 1, numcols)];
	Color *n7 = (image->image)[numcols * mod(row + 1, numrows) + col];
	Color *n8 = (image->image)[numcols * mod(row + 1, numrows) + mod(col + 1, numcols)];
	Color *neighbors[8];
	neighbors[0] = n1;
	neighbors[1] = n2;
	neighbors[2] = n3;
	neighbors[3] = n4;
	neighbors[4] = n5;
	neighbors[5] = n6;
	neighbors[6] = n7;
	neighbors[7] = n8;

	//create a new variable for R, G, B and allocate space for it
	//loop through the 8 bits for color R, G, B
	uint8_t newR = 0;
	for (int i = 0; i < 8; i++) {
		int bitonoff = (int) turnbit(cellclr, neighbors, i, 'R', rule);
		newR += bitonoff << i;
	}

	uint8_t newG = 0;
	for (int i = 0; i < 8; i++) {
		int bitonoff = (int) turnbit(cellclr, neighbors, i, 'G', rule);
		newG += bitonoff << i;
	}

	uint8_t newB = 0;
	for (int i = 0; i < 8; i++) {
		int bitonoff = (int) turnbit(cellclr, neighbors, i, 'B', rule);
		newB += bitonoff << i;
	}

	Color *newcell = (Color *) malloc(sizeof(Color));
	if (newcell == NULL) {
		fprintf(stderr, "Unable to allocate memory\n");
        exit(-1);
    }
	newcell->R = newR;
	newcell->G = newG;
	newcell->B = newB;
	return newcell;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
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
			Color *clr = evaluateOneCell(image, i, j, rule);
			imgptr[(image->cols)*i + j] = clr;
		}
	}
	newimg->image = imgptr;
	return newimg;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	if (argc != 3) {
		printf("%s\n", "usage: ./gameOfLife filename rule");
		printf("%s\n", "filename is an ASCII PPM file (type P3) with maximum value 255.");
		printf("%s\n", "rule is a hex number beginning with 0x; Life is 0x1808.");
		exit(-1);
	}
	char *filename = argv[1];
	FILE *fp;
	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Unable to open the file, the file does not exist");
		exit(-1);
	}

	char *lifegametype = argv[2];
	char *ptr;
	long lifegame = strtol(lifegametype, &ptr, 16);
	if (lifegame == 0) {
		fprintf(stderr, "The input for the rule is invalid");
	}

	Image *img = readData(filename);
	if (!img) {
		fprintf(stderr, "Unable to read file '%s'\n", filename);
		exit(-1);
	}
	Image *resultimg = life(img, lifegame);
	if (!resultimg) {
		fprintf(stderr, "Unable to read file '%s'\n", filename);
		exit(-1);
	}
	writeData(resultimg);
	freeImage(resultimg);
	freeImage(img);
}
