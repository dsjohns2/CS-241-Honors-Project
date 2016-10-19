/*	CS 241 Honors Project (Fall 2016)
	Steganography Tooltset
	Ask Anton
	Written: Oct 15-18
*/

// Generalization of my_encoder.cpp
// Currently has a bug (or a few) so that it correctly encodes
// only the EOF and messes up all other characters (but the number of them is correct).

#include "lodepng.cpp" // to make compilation simpler
#include "lodepng.h"

#include "encrypt.c"
#include "utilities.c"

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <assert.h>

#define BITS_PER_PIX 5 // parameter
#define DEBUG 0

#define CAN_BE_USED ((unsigned)((0b1<<BITS_PER_PIX)-1)) // deffinitely correct
#define SIGNIFICANT ((unsigned)(~CAN_BE_USED))			// for little endian machines

char * debug1 = NULL; char * debug2 = NULL;

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
	int character  = 0; // any value != EOF == -1
	while (character !=-1 && pixelIndex < width*height){
		character = fgetc (secretStream);

//assert (character == (int)(char)character);
if (DEBUG) printf ("Character is %i = %s\n", character, toBin (character, &debug2, 0));
if (DEBUG) printf ("Echar %c %s\n", character, toBin(character, &debug1, BITS_PER_PIX));

		for (unsigned bitsDone=0; bitsDone<sizeof(int)*8/*bits per byte*/; bitsDone+=BITS_PER_PIX){
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
		int character = 0;
		for (int bitsDone=0; bitsDone<sizeof(int)*8/*bits per byte*/; bitsDone+=BITS_PER_PIX){
			int newBits = image[pixelIndex] & CAN_BE_USED;
			character = character | (newBits<<bitsDone);
			pixelIndex++;
		}
		if (character==EOF) break;
		// !!! UNCOMMENT ME AND TRY TO FIX !!!
		fputc(character, secretStream);
//		if (fputc(character, secretStream))
//			errorMsg ((char*)"Decoder: Could not write to file.");
	}
}

int main(int argc, char *argv[]){
	assert (CAN_BE_USED + SIGNIFICANT == (unsigned)(~(unsigned)0));
	assert ((CAN_BE_USED & SIGNIFICANT) == 0);
	assert (sizeof(int)==sizeof(unsigned));
	FILE * secretStream = fopen ("ZrpZy2a.jpg", "r");
	streamEncoder ("husky.png", secretStream, "output2.png");
	fclose (secretStream);

	FILE * outSecretStream = fopen ("surprise.jpg", "w+");
	streamDecoder ("output2.png", outSecretStream);
	fclose (outSecretStream);



/*
	//	compare files
	FILE * original = fopen ("secretMessage.txt", "r");
	FILE * result   = fopen ("~secretMessage.txt", "r");
	dump_and_compare (original, result);
	fclose (original);
	fclose (result);
*/	
	if (DEBUG){ free (debug1); free (debug2); }
	return 0;
}
