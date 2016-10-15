// CS 241 Honors Project (Fall 2016)
// Steganography Tooltset
// Ask Anton Bershanskiy

#include <stdlib.h>

// Pseudo-random string generation
// Not secure for cryptographic purposes, but creates high-entropy data
// OK for testing
char * generatePad (size_t seed, const size_t length){
	char * pad = (char*) malloc (length);
	for (size_t index = 0; index<length; index++){
		seed = seed*seed + 1;
		char block = seed; // least-significant byte
		pad[index] = block;
	}
	return pad;
}

// XOR One-Time Pad
// needs to be vectorized
char * oneTimePadXOR (const char * plainData, const char * pad, const size_t length){
	char * cipherData = (char*) malloc (length);
	for (size_t index = 0; index < length; index++)
		cipherData [index] = plainData [index] ^ pad[index];
	return cipherData;
}
