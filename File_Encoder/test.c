void space_encoder(char* encode_str){
    //Open file
    void* file_to_encode = fopen("test.c", "r");
    void* encoded_file = fopen("output.c", "w");
    
    //Get line and encode
    char* line = NULL;
    int n = 0;
    int bytes_read;
    int cur_char_idx = 0;
    int num_spaces;
    while((bytes_read = getline(&line, &n, file_to_encode)) != -1){
        char* cur_line = strdup(line);
        cur_line[bytes_read-1] = 0;
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
            int org_line_length = strlen(cur_line);
            cur_line = realloc(cur_line, org_line_length+num_spaces+1);
            int i;
            for(i=org_line_length; i<org_line_length+num_spaces; i++){
                cur_line[i] = ' ';
            }
            cur_line[i] = 0;
            cur_char_idx++;
        }
        fprintf(encoded_file, "%s\n", cur_line);
        free(cur_line);
    }
    if(encode_str[cur_char_idx] != 0){
        //Did not reach end of encode_str
        printf("Warning: File not long enough to encode full string.\n");
    }
    free(line);
    fclose(file_to_encode);
    fclose(encoded_file);
}

int* get_binary_data(char* encode_str){
    int* ret = malloc(sizeof(int)*8*strlen(encode_str));
    for(int i=0; i<strlen(encode_str); i++){
        char cur_char = encode_str[i];
        for(int j=0; j<8; j++){
            ret[i*8 + j] = (cur_char>>j) & 1;
        }
    }
    return ret;
}


char* camelCaser(char* var){
    char* new_var = (char*) malloc(strlen(var)+1);
    memset(new_var, 0, strlen(var)+1);
    char* word_token = strtok(var, "_");
    for(unsigned long m = 0; m<strlen(word_token); m++){
        if(isalpha(word_token[m]+32)){
            word_token[m] = word_token[m]+32;
        }
    }
    while(word_token){
        int w;
        strcat(new_var, word_token);
        word_token = strtok(NULL, "_");
        if(word_token){
            for(w = 0; w<strlen(word_token); w++){
                if(word_token[0] < 91 && word_token[0] > 64){
                    break;
                }
                if(isalpha(word_token[w]-32)){
                    word_token[w] = word_token[w] - 32;
                    break;
                }
            }
            for(unsigned long m = w+1; m<strlen(word_token); m++){
                if(isalpha(word_token[m]+32) && isdigit(word_token[m]) == 0){
                    word_token[m] = word_token[m]+32;
                }
            }
        }
    }
    return new_var;
}


char* underscore(char* var){
    int upperCaseCount = 0;
    for(int i=0; i<strlen(var); i++){
        if(var[i] >=65 && var[i] <= 90){
            upperCaseCount++;
        }
    }
    char* new_var = (char*) malloc(strlen(var)+upperCaseCount+1);
    memset(new_var, 0, strlen(var)+upperCaseCount+1);
    int new_var_idx = 0;
    for(int var_idx = 0; var_idx < strlen(var); var_idx++){
        if(var[var_idx] >=65 && var[var_idx] <= 90){
            new_var[new_var_idx] = '_';
            new_var_idx++;
            new_var[new_var_idx] = var[var_idx] + 32;
            new_var_idx++;
        }
        else{
            new_var[new_var_idx] = var[var_idx];
            new_var_idx++;
        }
    }
    return new_var;
}


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


void variable_encoder(char* argv[], char* encode_str){
    //Write all encodable variables to a secret file (.variables)
    void* variables_file = fopen("./.variables", "w+");
    dup2(fileno(variables_file), fileno(stdout));
    Py_SetProgramName(argv[0]);
    Py_Initialize();
    int py_argc1 = 2;
    char* py_argv1[2] = {"./parser/pycparser/get_vars.py", "./test.c"};
    PySys_SetArgv(py_argc1, py_argv1);
    void* python_code1 = fopen("./parser/pycparser/get_vars.py", "r");
    PyRun_SimpleFile(python_code1, "./parser/pycparser/get_vars.py");
    Py_Finalize();

    //Check for good variables and set up to be camelCase
    void* good_variables_file = fopen("./.good_variables", "w+");
    int good_var_count = 0;
    char* lineptr = NULL;
    int n = 0;
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
    char* oldvars[good_var_count];
    char* newvars[good_var_count];
    int idx = 0;
    int new_good_var_count = good_var_count;
    while(getline(&lineptr, &n, good_variables_file) != -1){
        //Check for repeats
        lineptr[strlen(lineptr)-1] = 0;
        int flag = 0;
        for(int i=0; i<idx; i++){
            if(strcmp(lineptr, oldvars[i]) == 0){
                flag = 1;
                new_good_var_count--;
            }
        }
        if(flag){
            continue;
        }
        oldvars[idx] = strdup(lineptr);
        if(is_valid_var(lineptr)==1){
            newvars[idx] = strdup(camelCaser(lineptr));
        }
        else if(is_valid_var(lineptr)==2){
            newvars[idx] = strdup(lineptr);
        }
        idx++;
    }
    for(; idx<good_var_count; idx++){
        oldvars[idx] = NULL;
        newvars[idx] = NULL;
    }
    good_var_count = new_good_var_count;
    fclose(good_variables_file);

    //Get binary data of encoded message
    if(8*strlen(encode_str)>good_var_count){
        fprintf(stderr, "Not enough good variables to encode.\n");
    }
    int* binary_data = get_binary_data(encode_str);

    //Change elements of newvars according to binary data
    for(int i=0; i<good_var_count && i<(8*strlen(encode_str)); i++){
        if(binary_data[i] == 1){
            newvars[i] = underscore(newvars[i]);
        }
    }

    //Loop through variable arrays to change the target c code
    void* encoded_file;
    for(int i=0; i<good_var_count; i++){
        void* write_to_file = fopen("output_write.c", "w");
        dup2(fileno(write_to_file), fileno(stdout));
        char* oldvar = oldvars[i];
        char* newvar = newvars[i];
        fprintf(stderr, "%s -> %s\n", oldvars[i], newvars[i]);
        Py_SetProgramName(argv[0]);
        Py_Initialize();
        int py_argc2 = 4;
        char* py_argv2test[4] = {"./parser/pycparser/change_var.py", "./test.c", oldvar, newvar};
        char* py_argv2output[4] = {"./parser/pycparser/change_var.py", "./output_temp.c", oldvar, newvar};
        if(i==0){
            PySys_SetArgv(py_argc2, py_argv2test);
        }
        else{
            PySys_SetArgv(py_argc2, py_argv2output);
        }
        void* python_code2 = fopen("./parser/pycparser/change_var.py", "r");
        PyRun_SimpleFile(python_code2, "./parser/pycparser/change_var.py");
        Py_Finalize();
        fclose(write_to_file);
        child = fork();
        if(child == 0){
            execlp("cp", "cp", "./output_write.c", "./output_temp.c", NULL);
        }
        wait(NULL);
    }

    //Get rid of newlines at the end of the file
    encoded_file = fopen("output_temp.c", "r");
    void* output_file = fopen("output.c", "w");
    fseek(encoded_file, 0, SEEK_SET);
    while(getline(&lineptr, &n, encoded_file) != -1){
        //Reformats if too many newlines in middle of file
        if(strcmp(lineptr, "\n") == 0){
            if(getline(&lineptr, &n, encoded_file) != -1){
                if(strcmp(lineptr, "\n") == 0){
                    if(getline(&lineptr, &n, encoded_file) != -1){
                        if(strcmp(lineptr, "\n") == 0){
                            //print nothing
                        }
                        else{
                            fprintf(output_file, "%s", "\n");
                            fprintf(output_file, "%s", "\n");
                            fprintf(output_file, "%s", lineptr);
                        }
                    }
                }
                else{
                    fprintf(output_file, "%s", "\n");
                    fprintf(output_file, "%s", lineptr);
                }
            }
        }
        else{
            fprintf(output_file, "%s", lineptr);
        }
    }
    fclose(encoded_file);
    fclose(output_file);

    //Cleanup
    remove("./output_temp.c");
    remove("./output_write.c");
    remove("./.variables");
    remove("./.good_variables");
        //TODO: clean up the arrays
}

int main(int argc, char* argv[]){
    //Set up string to encode
    char* lineptr = NULL;
    int n = 0;
    int bytes_read;

    //Pick encoding
    printf("Space or Variable Encoding: ");
    bytes_read = getline(&lineptr, &n, stdin);
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
    if(encode_option == 1){
        //Encode space
        printf("Space encoding %s into output.c\n", lineptr);
        space_encoder(lineptr);
    }
    else if(encode_option == 2){
        //Encode variable
        printf("Variable encoding %s into output.c\n", lineptr);
        variable_encoder(argv, lineptr);
    }

    //Print completete
    fprintf(stderr, "Finished\n");

    //Clean up
    free(lineptr);

    //Return
    return 0;
}
