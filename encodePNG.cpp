/*	CS 241 Honors Project (Fall 2016)
	Steganography Tooltset
	Ask Anton
	Written: Oct 15-18
*/

#include "lodepng.cpp" // to make compilation simpler
#include "lodepng.h"

#include "utilities.c"

#include <vector>
//#include <stdio.h>
//#include <stdlib.h>
//#include <iostream>

#define BITS_PER_PIX 4 // parameter
#define DEBUG 0

#define CAN_BE_USED ((unsigned)((0b1<<BITS_PER_PIX)-1)) // deffinitely correct
#define SIGNIFICANT ((unsigned)(~CAN_BE_USED))			// for little endian machines

char * debug1, * debug2;

// 1 if close
unsigned compare (std::vector<unsigned char> image, unsigned index1, unsigned index2){
	return ((SIGNIFICANT & image[index1]) == (SIGNIFICANT & image[index2]));
}

// Returns 0 if we should not use it, 1 if it's Ok
unsigned variation (std::vector<unsigned char> image, unsigned index, unsigned width, unsigned height){
	unsigned line   = index / width;
	unsigned column = index % width;
	char result = 1;
	if (   (      line     && compare (image, index, index-width))	// above
		|| (      column   && compare (image, index, index-1))		// left
		|| (line<height-1   && compare (image, index, index+width))	// below
		|| (column<width-1 && compare (image, index, index+1)))	// right
		return 0;
		else return 1;
}

// Big endian
// Encoder is correct, uncomment the debug stuff to make sure
void streamEncoder (const char * ballastImage, FILE * secretStream, const char * saveAs){
	std::vector<unsigned char> image;
	unsigned width, height, error;
	error = lodepng::decode (image, width, height, ballastImage);	// Decode png to vector
	unsigned pixelIndex = 0;
	char character  = 0; // any value != EOF == -1
	while (fread (&character, sizeof(character), 1, secretStream) && pixelIndex < width*height){

if (DEBUG) printf ("Character is %i = %s\n", character, toBin (character, &debug2, 0));
if (DEBUG) printf ("Echar %c %s\n", character, toBin(character, &debug1, BITS_PER_PIX));

		for (unsigned bitsDone=0; bitsDone<sizeof(character)*8/*bits per byte*/; bitsDone+=BITS_PER_PIX){
			unsigned newPixel = (SIGNIFICANT & image[pixelIndex]) | (CAN_BE_USED & character);

if (DEBUG) printf ("   ---pixel change---\n   from %s\n    to  %s\n", toBin (image[pixelIndex], &debug1, BITS_PER_PIX), toBin (newPixel, &debug2, BITS_PER_PIX));

			image[pixelIndex] = newPixel;
			character >>= BITS_PER_PIX;
			pixelIndex++;
		}
	}

	error = lodepng::encode (saveAs, image, width, height); 	// Turn vector back into png
}

// Correctly decodes package Image but can not write to the output stream
void streamDecoder (const char * packageImage, FILE * secretStream){
	std::vector<unsigned char> image;
	unsigned width, height, error;
	error = lodepng::decode (image, width, height, packageImage);	//Decode png to vector

	unsigned pixelIndex = 0;
	while (pixelIndex < width*height){
		char character = 0;
		for (int bitsDone=0; bitsDone<sizeof(character)*8/*bits per byte*/; bitsDone+=BITS_PER_PIX){
			int newBits = image[pixelIndex] & CAN_BE_USED;
			character = character | (newBits<<bitsDone);
			pixelIndex++;
		}
//		if (character==EOF) break;
fwrite (&character, sizeof(character), 1, secretStream);
		// !!! UNCOMMENT ME AND TRY TO FIX !!!
//		fputc(character, secretStream);
//		if (fputc(character, secretStream)){
//			printf ("char %i %c", character, character);
//			errorMsg ((char*)"Decoder: Could not write to file.");
//		}
	}
}
