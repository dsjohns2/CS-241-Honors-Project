/*	CS 241 Honors Project (Fall 2016)
	Steganography Tooltset
	Ask Anton
	Written: Oct 15
*/

// Generalization of my_encoder.cpp
// Currently has a bug (or a few) so that it correctly encodes
// only the EOF and messes up all other characters (but the number of them is correct).

#include "lodepng.h"
#include "encrypt.c"

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <assert.h>

#define BITS_PER_PIX 4 // parameter

#define CAN_BE_USED ((unsigned)((0b1<<BITS_PER_PIX)-1)) // deffinitely correct
#define SIGNIFICANT ((unsigned)(~CAN_BE_USED))			// for little endian machines

// 1 if close
unsigned compare (std::vector<unsigned char> image, unsigned index1, unsigned index2){
	return (SIGNIFICANT & image[index1] == SIGNIFICANT & image[index2]);
}

// Returns 0 if we should not use it, 1 if it's Ok
unsigned variation (std::vector<unsigned char> image, unsigned index, unsigned width, unsigned height){
	unsigned line   = index / width;
	unsigned column = index % width;
	char result = 1;
	if (   (      line     && compare (image, index, index-width))	// above
		|| (      column   && compare (image, index, index-1))		// left
		|| (line<heigh-1   && compare (image, index, index+width))	// below
		|| (column<width-1 && compare (image, index, index+1)))	// right
		return 0;
		else return 1;
}

void errorMsg (char * errorMessage){
	printf ("%s\n", errorMessage);
	exit (1);
}

void streamEncoder (const char * ballastImage, FILE * secretStream, const char * saveAs){
	std::vector<unsigned char> image;
	unsigned width, height, error;
	error = lodepng::decode (image, width, height, ballastImage);	//Decode png to vector
	//Encode
	unsigned pixelIndex = 0;
	int character  = fgetc (secretStream);
		assert (character == (char)character);
	char notEOFc = 1;
	while (notEOFc && pixelIndex < width*height){
		if (character==EOF)	// exactly one EOF is printed
			notEOFc=0;
		//printf ("Enc char %c\n", character);
		for (unsigned bitsDone=0; bitsDone<sizeof(int)*8/*bits per byte*/; bitsDone+=BITS_PER_PIX){
			unsigned newPixel = (SIGNIFICANT & image[pixelIndex]) | (CAN_BE_USED & character);
			image[pixelIndex] = newPixel;
			character >>= BITS_PER_PIX;
			pixelIndex++;
		}
		character = fgetc (secretStream);
	}

	//Turn vector back into png
	error = lodepng::encode (saveAs, image, width, height); 
}

void streamDecoder (const char * packageImage, FILE * secretStream){
	std::vector<unsigned char> image;
	unsigned width, height, error;
	error = lodepng::decode (image, width, height, packageImage);	//Decode png to vector

	unsigned pixelIndex = 0;
	int character  = 1;
	while (pixelIndex < width*height){
		character = 0;
		for (int bitsDone=0; bitsDone<sizeof(int)*8/*bits per byte*/; bitsDone+=BITS_PER_PIX){
			int newBits = image[pixelIndex] & CAN_BE_USED;
			character = (character<<BITS_PER_PIX) | newBits;
			pixelIndex++;
		}
		if (character==EOF) break;
		if (fputc(character, secretStream)){
			printf ("Bad char '%c'=%i", character, (int)character);
			errorMsg ((char*)"Decoder: Could not write to file.");
		}
	}
}

char * inputPrompt (char * message){
	printf ("%s", message);
	char * response = NULL;
	size_t length;
	if (getline (&response, &length, stdin) != -1){
		response [strlen(response)-1] = 0; // delete new line char //WHY IS LENGTH SO STRANGE?
		return response;
	} else {
		free (response);
		return NULL;
	}
}

int main(int argc, char *argv[]){
	assert (CAN_BE_USED + SIGNIFICANT == (unsigned)(~(unsigned)0));
	assert ((CAN_BE_USED & SIGNIFICANT) == 0);
	assert (sizeof(int)==sizeof(unsigned));
	FILE * secretStream = fopen ("secretMessage.txt", "r");
	streamEncoder ("test.png", secretStream, "output2.png");
	fclose (secretStream);
	
	FILE * outSecretStream = fopen ("~secretMessage.txt", "w+");
	streamDecoder ("output2.png", outSecretStream);
	fclose (outSecretStream);
	
//	// compare files
//	FILE * original = fopen ("secretMessage.txt", "r");
//	FILE * result   = fopen ("~secretMessage.txt", "r");
//	fclose (original);
//	fclose (result);
	
	return 0;
}