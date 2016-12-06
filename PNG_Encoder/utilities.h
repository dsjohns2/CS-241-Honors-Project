// CS  Honors Project (Fall 2016)
// Some Utility functions for PNG encoder debuging
// Ask Anton Bershanskiy
// Oct 15-18

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Some very helpful functions for debuging encodePNG

// Endiedness of the system: bigendian vs littleendian
// 1 if little endian, 0 if big endian
char littleEndian();

// unifies error messages printout
void errorMsg (const char * errorMessage);

char * inputPrompt (char * message);


// Returns dinamically alocated string of '0' and '1' which is a binary representation of n
// separates bits_per_component last characters with a space
char * toBin (int n, char ** buffer, int bits_per_component);

// Prints out two by a single character and corresponding binary representation
void dump_and_compare (FILE * one, FILE * two, const int bitsPerComponent);
