// CS 241 Honors Project (Fall 2016)
// Steganography Tooltset
// Ask Anton Bershanskiy

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// Pseudo-random string generation
// Not secure for cryptographic purposes, but creates high-entropy data
// OK for testing
// return error code
int generatePad (size_t seed, const size_t length, FILE * pad){
	char * localPad = (char*) malloc (length);
	for (size_t index = 0; index<length; index++){
		seed = seed*seed + 1;
		char block = *(((char*)&seed)+7); // least-significant byte
		localPad [index] = block;
//printf ("Pad %zu %zu\n", seed, (size_t)block);
	}
	size_t wrote = fwrite (localPad, 1, length, pad);
	assert (wrote == length);
	return 0;
}

// XOR One-Time Pad
// needs to be vectorized
int oneTimePadXOR (FILE * argument1, FILE * argument2, FILE * output, const size_t length){
	char * a = (char*) malloc (length);
	char * b = (char*) malloc (length);
	char * c = (char*) malloc (length);
	size_t read1 = fread (a, 1, length, argument1);
	size_t read2 = fread (b, 1, length, argument2);
	assert (read1 == length && read2 == length);
	for (size_t index = 0; index < length; index++)
		c [index] = a [index] ^ b [index];
	size_t wrote = fwrite (c, 1, length, output);
	assert (wrote == length);
	return 0;
}
