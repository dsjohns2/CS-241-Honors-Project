#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string.h>
#include <Python.h>

int is_valid_var(char* in){
    //Local variables
    char* var = strdup(in);

    //Check for underscores
    for(int i=0; i<strlen(var); i++){
        if(var[i] == '_'){
            return 1;
        }
    }

    //First letter is not capitalized, but there is at least one capital letter
    for(int i=1; i<strlen(var); i++){
        if(var[i] >= 65 && var[i] <= 90){
            return 2;
        }
    }

    //Clean up
    free(var);

    //Return
    return 0;
}

void variable_decoder(char* argv[]){
    //Write all encodable variables to a secret file (.variables)
    FILE* variables_file = fopen("./.variables", "w+");
    int saved_stdout = dup(1);
    dup2(fileno(variables_file), fileno(stdout));
    Py_SetProgramName(argv[0]);
    Py_Initialize();
    int py_argc1 = 2;
    char* py_argv1[2] = {"./parser/pycparser/get_vars.py", "./output.c"};
    PySys_SetArgv(py_argc1, py_argv1);
    FILE* python_code1 = fopen("./parser/pycparser/get_vars.py", "r");
    PyRun_SimpleFile(python_code1, "./parser/pycparser/get_vars.py");
    Py_Finalize();

    //Check for good variables and set up to be camelCase
    FILE* good_variables_file = fopen("./.good_variables", "w+");
    int good_var_count = 0;
    char* lineptr = NULL;
    size_t n = 0;
    fseek(variables_file, 0, SEEK_SET);
    while(getline(&lineptr, &n, variables_file) != -1){
        int pass = is_valid_var(lineptr);
        if(pass){
            fprintf(good_variables_file, "%s", lineptr);
            good_var_count++;
        }
    }
    fclose(variables_file);

    //Set up oldvars and newvars arrays
    fseek(good_variables_file, 0, SEEK_SET);
    char* vars[good_var_count];
    int idx = 0;
    int new_good_var_count = good_var_count;
    while(getline(&lineptr, &n, good_variables_file) != -1){
        //Check for repeats
        lineptr[strlen(lineptr)-1] = 0;
        int flag = 0;
        for(int i=0; i<idx; i++){
            if(strcmp(lineptr, vars[i]) == 0){
                flag = 1;
                new_good_var_count--;
            }
        }
        if(flag){
            continue;
        }
        vars[idx] = strdup(lineptr);
        idx++;
    }
    for(; idx<good_var_count; idx++){
        vars[idx] = NULL;
    }
    good_var_count = new_good_var_count;
    fclose(good_variables_file);

    //Decode message
    dup2(saved_stdout, 1);
    int next_char = 0;
    for(int i=0; i<good_var_count; i++){
        if(i%8 == 0 && i != 0){
            printf("%c", next_char);
            next_char = 0;
        }
        int bit = 0;
        char* cur_var = vars[i];
        for(int j=0; j<strlen(cur_var); j++){
            if(cur_var[j] == '_'){
                bit = 1;
                break;
            }
        }
        if(bit){
            int add_on = 1;
            for(int j=0; j<(i%8); j++){
                add_on *= 2;
            }
            next_char += add_on;
        }
    }
    printf("\n");

    //Clean up
    remove("./.variables");
    remove("./.good_variables");
}

void space_decoder(){
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
}

int main(int argc, char* argv[]){
    //Set up string to decode
    char* lineptr = NULL;
    size_t n = 0;

    //Pick encoding
    printf("Space or Variable Encoding: ");
    getline(&lineptr, &n, stdin);
    int encode_option;

    if(strcmp(lineptr, "Space\n") == 0){
        encode_option = 1;
    }
    else if(strcmp(lineptr, "Variable\n") == 0){
        encode_option = 2;
    }
    else{
        fprintf(stderr, "Invalid encoding option.\n");
        encode_option = -1;
        return 1;
    }

    //Encode
    if(encode_option == 1){
        //Decode space
        space_decoder();
    }
    else if(encode_option == 2){
        //Decode variable
        variable_decoder(argv);
    }

    //Clean up
    free(lineptr);

    //Return
    return 0;
}
