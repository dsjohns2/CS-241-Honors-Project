#include "lodepng.h"

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
/*
3 ways to decode a PNG from a file to RGBA pixel data (and 2 in-memory ways).
*/

/*
Example 1
Decode from disk to raw pixels with a single function call
*/

void decodeOneStep(const char* filename)
{
  std::vector<unsigned char> image; //the raw pixels
  unsigned width, height;
 //decode
  unsigned error = lodepng::decode(image, width, height, filename);
  printf("Image Dimensions : %d X %d\n", width, height);
  size_t i = 0;
  //image++;
  printf("Whatt : %x\n", image);
  //if there's an error, display it
  if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

  //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
}
int main(int argc, char *argv[])
{
  const char* filename = argc > 1 ? argv[1] : "test.png";

  decodeOneStep(filename);

  return 0;
}
