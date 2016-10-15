#include "lodepng.h" // reading and writing PNG files
#include "encrypt.c" // our own implementation of simple cryptographic primitives

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

/*
Example 1
Decode from disk to raw pixels with a single function call
*/

void decodeOneStep(const char* filename)
{
  std::vector<unsigned char> image; //the raw pixels
  unsigned width, height;
 //decode
  unsigned error = lodepng::decode(image, width, height, filename);  //decode stores pixel data in vector<unsigned char>, using 4 of those for every pixel, one for each channel
  printf("Image Dimensions : %d X %d\n", width, height);
  size_t i = 0;
  //Print out the middle chunk.
  for(i = 500000; i < 510000; i++)
   {
     printf("%02x", image[i]);
     if(i != 0 && i%4 == 0)
     printf(" ");
   }
  //if there's an error, display it
  if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

  //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
}



int main(int argc, char *argv[]){
	const char* filename = argc > 1 ? argv[1] : "test.png";

  decodeOneStep(filename);

  return 0;
}
