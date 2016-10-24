/*
Simple WAV encoder/decoder.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

//This structure will hold the infomation encoded in the header of .wav file
//The header has always an size of 44 bytes.
typedef struct wav_meta_data
{
 char RIFF[4];			//Must contain "RIFF"
 int32_t fileSize;		//File size of the current file
 char WAVE[4];			//Must contain "WAVE"
 char fmt[4];			//Format Chunk Marker, contains "fmt "
 int32_t formatLength;		//Length of format
 int16_t formatType;		//Type of format. 1 is PCM, which is uncompressed
 int16_t numChannels; 		//1 : MONO, 2 : Stereo
 int32_t sampleRate;		//Number of samples per second.. 44.1KHZ, 48KHZ etc
 int32_t avgBytes;		//Average number of bytes/second. (sampleRate * BitsPerSample * numChannels)/8
 int16_t overallBytesPerSample;	//Holds (BytesPerSample * numChannels)
 int16_t BitsPerSample;		//Bits per sample :|
 char dataChunk[4];		//Contains "data", Marks the beginning of data section;
 int32_t dataSize;		//Size of the data section
  
} wav_meta_data;

//Check the function definition for usage and purpose
void printBytes(char*, size_t);
void printInfo(wav_meta_data);
void encode(FILE*, FILE*, char*, int32_t);
void decoder(FILE*, FILE*, int32_t);


//---------------MAIN-------------------
int main(int argc, char** argv)
{

 //USAGE-----------
 if(argc != 4)
  {
   printf("Usage : ./name.. <encode/decode> <inputFile/orginalFile> <outputFile/encodedFile>\n");
   exit(1);
  }


 int flag = 2;
 wav_meta_data data;
 //Open the .wav File
 FILE* file = fopen(argv[2], "r");
 //Read the meta_data from file into memory
 fread(&data, sizeof(wav_meta_data), 1, file);
 //Create output file.
 FILE* outputFile = NULL;

 //Ugly option selection
 //TODO : Make a better interface
 if(strcmp("encode", argv[1]) == 0)
 {
 printf("Encoding...\n");
 flag = 0;
 outputFile = fopen(argv[3], "w");
 fwrite(&data, sizeof(wav_meta_data), 1, outputFile);
 }
 else
 {
  flag = 1;
  printf("Decoding..\n");
  outputFile = fopen(argv[3], "r");
  //Decoder and Encode require file streams to be pointing at the start of the data section.
  fseek(outputFile, sizeof(wav_meta_data), SEEK_SET);
 }

 //Print metadata info
 printInfo(data);

 //Message we are encoding.....
 char plainText[13] = "Hello World!";

 //ENCODE
 if(flag == 0)
 encode(file, outputFile, plainText, data.dataSize);
 //DECODE
 else if(flag == 1)
 decoder(outputFile, file, data.dataSize);
 
 //Close the file streams.
 fclose(file);
 fclose(outputFile);
 return 0;
}


//Helper function to print Non-Null Terminated strings. Provide Stream and Number of characters to print.
void printBytes(char* buffer, size_t numBytes)
{
 size_t i = 0;
 for(i = 0; i < numBytes; i++)
  putchar(buffer[i]);
 printf("\n");
}




//Print WAV File information
void printInfo(wav_meta_data data)
 {
  //Need to print character wise because strings in meta_data are not NULL terminated.
  printf("RIFF value : ");
  printBytes(data.RIFF, 4);
  printf("File size is : %d\n", data.fileSize);
  printf("WAVE value : ");
  printBytes(data.WAVE, 4);
  printf("fmt value : ");
  printBytes(data.fmt, 4);
  printf("Format length is : %d\n", data.formatLength);
  printf("Format type is (1 means PCM) : %d\n", data.formatType);
  printf("Number of channels : %d\n", data.numChannels);
  printf("Sample Rate in Hz : %d\n", data.sampleRate);
  printf("Average number of bytes per second : %d\n", data.avgBytes);
  printf("Overall bytes per sample : %d\n", data.overallBytesPerSample);
  printf("Bits per Sample (per channel) : %d\n", data.BitsPerSample);
  printf("Data chunk value : ");
  printBytes(data.dataChunk, 4);
  printf("Data section size : %d\n", data.dataSize);
 }




//This function will create a copy of input file as output, and at the same time, encode the given plainText message into the file.
//NOTE : The file pointers are expected to be pointing to right at the beginning of data segment.
//TODO : Make it buffered
void encode(FILE* input, FILE* output, char* plainText, int32_t fileSize)
{
 //Tried to make it buffered, but my cold isn't letting me think -_-
 size_t bytesWritten = 0;
 size_t bytesEncoded = 0;
 int count = 0;
 size_t i = 0;
 size_t length = strlen(plainText);
 
 int buffer[1];
 while(bytesWritten <= fileSize)
 {
  //Read an int from original audio file
  fread(&buffer, sizeof(int), 1, input);
  	
  //Encoding character wise., just adding the ASCII value for now..  	
	if(count < length)
    	buffer[0] += plainText[count];
  
  //Write an int to new audio file
  fwrite(&buffer, sizeof(int), 1, output);
  bytesWritten += 1;
  count++;
 }
 
} 




//This function will take the encoded file,and original file and find difference between them, and print if the value isn't 0
//NOTE : The file pointers are expected to be pointing to right at the beginning of data segment.
//TODO : Make it buffered..
void decoder(FILE* encodedFile, FILE* originalFile, int32_t fileSize)
{
 size_t counter = 0;
 int encoded;
 int original;
 while(counter != 13)
 {
  	counter++;
  	//Read an int from encoded audio
  	fread(&encoded, sizeof(int), 1, encodedFile);
	//Read an int from orginal audio
  	fread(&original, sizeof(int), 1, originalFile);
  	int val = encoded - original;
 	 	if(val != 0)
  	 	putchar(val);
 }
putchar('\n');
}
