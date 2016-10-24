#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void condensed_encoder(char* encode_str){
    //Open file
    FILE* file_to_encode = fopen("test.c", "r");
    FILE* encoded_file = fopen("output.c", "w");
    
    //Get line and encode
    char* line = NULL;
    size_t n = 0;
    size_t bytes_read;
    int cur_char_idx = 0;
    int num_spaces;
    while((bytes_read = getline(&line, &n, file_to_encode)) != -1){
        line[bytes_read-1] = 0;
        char cur_char = encode_str[cur_char_idx];
        cur_char = toupper(cur_char);
        if(cur_char != 0){
            //Not yet reached end of encode_str
            if(cur_char == 32){
                //Space character special case
                num_spaces = 27;
            }
            else if(cur_char == 46){
                //Period character special case
                num_spaces = 28;
            }
            else if((cur_char>64 && cur_char<91) || (cur_char>96 && cur_char<123)){
                //Normal letter case
                num_spaces = cur_char - 64;
            }
            else{
                //Invalid character case
                printf("Invalid character\n");
                break;
            }
            int org_line_length = strlen(line);
            line = realloc(line, org_line_length+num_spaces+1);
            int i;
            for(i=org_line_length; i<org_line_length+num_spaces; i++){
                line[i] = ' ';
            }
            line[i] = 0;
        }
        fprintf(encoded_file, "%s\n", line);
        cur_char_idx++;
    }
    if(encode_str[cur_char_idx] != 0){
        //Did not reach end of encode_str
        printf("Warning: File not long enough to encode full string.\n");
    }
    free(line);
    fclose(file_to_encode);
    fclose(encoded_file);
}

int main(){
    //Set up string to encode
    char* lineptr = NULL;
    size_t n = 0;
    size_t bytes_read;

    //Read user input
    printf("Only letters, periods, and spaces are currently supported.\n");
    printf("Input string to encode: ");
    bytes_read = getline(&lineptr, &n, stdin);

    //Getline error
    if(bytes_read == -1){
        exit(1);
    }

    //Get rid of newline character
    lineptr[strlen(lineptr)-1] = 0;

    //Encode
    printf("Encoding %s into output.c\n", lineptr);
    condensed_encoder(lineptr);
    free(lineptr);

    //Return
    return 0;
}
