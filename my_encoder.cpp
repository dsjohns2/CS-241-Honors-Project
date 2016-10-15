#include "lodepng.h"

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

void my_encoder(const char* filename, char* in){
    //Local Variables
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error;

    //Decode png to vector
    error = lodepng::decode(image, width, height, filename);

    //TODO: decode error checker

    //Encode
    int j=-1;
    for(int i=0; i < width*height; i++){
        //Stay on this character for 8 iterations
        if(i%8 == 0){
            j++;
        }
        
        //Break if out of characters to encode
        if(j==strlen(in)){
            break;
        }

        //Current character
        char current_byte = in[j];

        //Get bit that I am encoding
        char and_with_me = 0x01;
        and_with_me <<= i%8;
        char bit = and_with_me & current_byte;
        bit >>= i%8;

        //This is for the case that 10000000 is right shifted
        bit &= 0x01;

        //Encode bit into image
        image[i] |= bit;
    }

    //Turn vector back into png
    error = lodepng::encode("test_encoded.png", image, width, height); 
    
    //TODO: encode error checker
    
}

int main(int argc, char *argv[]){
    //Set up string to encode
    char* lineptr = NULL;
    size_t n = 0;
    size_t bytes_read;

    //Read user input
    printf("Input string to encode: ");
    bytes_read = getline(&lineptr, &n, stdin);

    //Getline error
    if(bytes_read == -1){
        exit(1);
    }

    //Get rid of newline character
    lineptr[strlen(lineptr)-1] = 0;

    //Encode
    my_encoder("test.png", lineptr);
    free(lineptr);
    return 0;
}
