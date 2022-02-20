# GameofLife

In imageloader.c, functions readData, writeData, and freeImage are used to convert between the Image struct and .ppm files. readData reads an Image struct from a .ppm file, writeData writes an Image struct in its PPM format to standard output, and freeImage frees an Image object.

In steganography.c, implemented evaluateOnePixel, steganography, and main and decoded the given secret message. The program takes in a .ppm file as input and print out a corresponding “decoded” image.

make steganography compiles steganography and outputs the result in studentOutputs/secretmessage.ppm. make steganographymemcheck can also be used to test for memory leaks.


The Game of Life is usually played on a bitmap (black and white) image over many timesteps, in which each pixel (or bit) is either 1 (alive) or 0 (dead). The original rules for the Game of Life are reprinted below:

1. Any live cell with two or three live neighbours (in the surrounding 8 cells) survives to the next generation.
2. Any dead cell with three live neighbours becomes a live cell in the next generation.
3. All other live cells die in the next generation. Similarly, all other dead cells stay dead.

To expand upon the standard bitmap Game of Life, the implementation reads in an ASCII full-color (24-bit) image and the rules for the variant of the Game of Life you wish to simulate, and print (to STDOUT) another ASCII full-color image of one iteration of the simulation. What makes this neat is that you’ll be running 24 simultaneous Game of Life simulations, one for every “bit plane” of the 24-bit (8 Red, 8 Blue, and 8 Green) image.



