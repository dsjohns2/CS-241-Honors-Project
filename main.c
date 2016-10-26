
//#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//#include "utilities.c"

//#include "encrypt.c"
//#include "encodePNG.cpp"
//
// The "UI" stuff for the encoder
void streamEncoder (const char * ballastImage, FILE * secretStream, const char * saveAs);
void streamDecoder (const char * packageImage, FILE * secretStream);
char * inputPrompt (char * message);



int main(int argc, char *argv[]){

	char * secretFile, * ballastImage, * packageImage, * outputFile;
	if (argc==1){
		secretFile   = inputPrompt ((char*) "The secret file:  ");
		ballastImage = inputPrompt ((char*) "The ballsast PNG: ");
		packageImage = inputPrompt ((char*) "Save package as:  ");
		outputFile   = inputPrompt ((char*) "Save output as:   ");
	} else if (argc == 5){
		secretFile   = argv[1];
		ballastImage = argv[2];
		packageImage = argv[3];
		outputFile   = argv[4];
	} else {printf ("Incorrect usage:\n      ./run (to input the filenames manually)\n      ./run [secret (in)] [ballast (in)] [package (out)] [decode (out)]\n");exit (1);}

	FILE * secretStream = fopen (secretFile, "r");
	FILE * outSecretStream = fopen (outputFile, "w+");

/*
	// generate pad
	FILE * padStream = fopen ("oneTimePad.bin", "w+");
	rewind (padStream);
	generatePad (0x40, 100000000000000, padStream);
	rewind (padStream);
*/

//	int encoding[2]; // 0 is read, 1 write
//	/*int*/ pipe (encoding);
//	int decoding[2]; // 0 is read, 1 write
//	/*int*/ pipe (decoding);

//	oneTimePadXOR (secretStream, padStream, fdopen(encoding[1], "w"), 100000000000000);
//	rewind (padStream);

	streamEncoder (ballastImage, secretStream/*fdopen(encoding[0], "r")*/, packageImage);
	fclose (secretStream);

	streamDecoder (packageImage, outSecretStream/*fdopen(decoding[1], "w")*/);

//	oneTimePadXOR (fdopen(decoding[0], "r"), padStream, outSecretStream, 1000000000);
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
