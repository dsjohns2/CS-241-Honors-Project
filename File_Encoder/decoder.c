#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(){
    //Open file
    FILE* file_to_decode = fopen("output.c", "r");
    
    //Get line and decode
    char* line = NULL;
    size_t n = 0;
    size_t bytes_read;
    while((bytes_read = getline(&line, &n, file_to_decode)) != -1){
        line[bytes_read-1] = 0;
        int num_spaces = 0;
        for(int i=strlen(line)-1; line[i]==' '; i--){
            num_spaces++;
        }
        if(num_spaces==0){
            //Nothing more to decode
            printf("\n");
            break;
        }
        else if(num_spaces == 27){
            //Space special case
            printf("%c", 32);
        }
        else if(num_spaces == 28){
            //Space special case
            printf("%c", 46);
        }
        else{
            //Normal letter case
            printf("%c", num_spaces+64);
        }
    }
    free(line);
    fclose(file_to_decode);
    
    //Return
    return 0;
}
