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
#include "utilities.c"

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <assert.h>

#define BITS_PER_PIX 4 // parameter
#define DEBUG 0

#define CAN_BE_USED ((unsigned)((0b1<<BITS_PER_PIX)-1)) // deffinitely correct
#define SIGNIFICANT ((unsigned)(~CAN_BE_USED))			// for little endian machines
#define ENCODE_MASK 0x1000

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

void errorMsg (char * errorMessage){
	printf ("%s\n", errorMessage);
	exit (1);
}

// Big endian
// Encoder is correct, uncomment the debug stuff to make sure
void streamEncoder (const char * ballastImage, FILE * secretStream, const char * saveAs){
		if (DEBUG)	debug1 = NULL; debug2 = NULL;
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
				if (DEBUG){ debug1=toBin(character, debug1, BITS_PER_PIX); printf ("Echar %c %s\n", character, debug1);}
		for (unsigned bitsDone=0; bitsDone<sizeof(int)*8/*bits per byte*/; bitsDone+=BITS_PER_PIX){
			unsigned newPixel = (SIGNIFICANT & image[pixelIndex]) | (CAN_BE_USED & character);
			//unsigned newPixel = (SIGNIFICANT & image[pixelIndex]) | (ENCODE_MASK & character);
					if (DEBUG){ debug1 = toBin (image[pixelIndex], debug1, BITS_PER_PIX); debug2 = toBin (newPixel, debug2, BITS_PER_PIX);
							printf ("   ---pixel change---\n   from %s\n    to  %s\n", debug1, debug2);}
			image[pixelIndex] = newPixel;
			character >>= BITS_PER_PIX;//<<
			pixelIndex++;
		}
		character = fgetc (secretStream);
	}

	//Turn vector back into png
	error = lodepng::encode (saveAs, image, width, height); 
		if (DEBUG) {free (debug1);free(debug2);}
}

// Correctly decodes package Image but can not write to the output stream
void streamDecoder (const char * packageImage, FILE * secretStream){
			if (DEBUG) debug1 = NULL;
	std::vector<unsigned char> image;
	unsigned width, height, error;
	error = lodepng::decode (image, width, height, packageImage);	//Decode png to vector

	unsigned pixelIndex = 0;
	int character;
	while (pixelIndex < width*height){
		character = 0;
		for (int bitsDone=0; bitsDone<sizeof(int)*8/*bits per byte*/; bitsDone+=BITS_PER_PIX){
			int newBits = image[pixelIndex] & CAN_BE_USED;
			character = character | (newBits<<bitsDone);
				if (DEBUG) {debug1 = toBin (character, debug1, BITS_PER_PIX); printf ("Dchar %c = %s\n", character, debug1);}
			pixelIndex++;
		}
		if (character==EOF) break;
		// !!! UNCOMMENT ME AND TRY TO FIX !!!
		fputc(character, secretStream);
//		if (fputc(character, secretStream))
//			errorMsg ((char*)"Decoder: Could not write to file.");
				if (1) {debug1 = toBin (character, debug1, BITS_PER_PIX);printf ("Got char %c = %s\n", character, debug1); }
	}
			free(debug1);
}

// I think it is little endin, but it could just be buggy big endian 
// I'm not sure (Anton)
void streamDecoder1 (const char * packageImage, FILE * secretStream){
			if (DEBUG) debug1 = NULL;
	std::vector<unsigned char> image;
	unsigned width, height, error;
	error = lodepng::decode (image, width, height, packageImage);	//Decode png to vector

	unsigned pixelIndex = 0;
	int character;
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
				if (DEBUG) { debug1 = toBin (character, debug1, BITS_PER_PIX);printf ("Got char %c = %s\n", character, debug1);}
	}
			if (DEBUG) free(debug1);
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

// unfinished function
void dump_and_compare (FILE * one, FILE * two){
	printf ("Comparing two files:\n");
	int char1 = getc (one);
	int char2 = getc (two);
			char * c1 = NULL; char * c2 = NULL;
	while (char1!=EOF && char2!=EOF){
				c1 = toBin (char1, c1, BITS_PER_PIX);
				c2 = toBin (char2, c2, BITS_PER_PIX);
		printf ("--------------------\n %c = %s\n %c = %s\n", char1, c1, char2, c2);

		char1 = getc (one); char2 = getc (two);
	}
			free (c1); free(c2);
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
	
/*
	//	compare files
	FILE * original = fopen ("secretMessage.txt", "r");
	FILE * result   = fopen ("~secretMessage.txt", "r");
	dump_and_compare (original, result);
	fclose (original);
	fclose (result);
*/	
	return 0;
}