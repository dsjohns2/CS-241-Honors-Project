// CS  Honors Project (Fall 2016)
// Some Utility functions that might be handy
// Ask Anton Bershanskiy
// Oct 15-18

#include <unistd.h>

// Endiedness of the system: Bigendien vs littleendian
// 1 if little endian, 0 if big endian
char littleEndian(){
	int a = 1;
	return *(char*)&a;
}


// uname -m (-a)

void errorMsg (char * errorMessage){
	printf ("%s\n", errorMessage);
	exit (1);
}

char * inputPrompt (char * message){
	printf ("%s", message);
	char * response = NULL;
	size_t length;
	if (getline (&response, &length, stdin) != -1){
		response [strlen(response)-1] = 0; // delete new line char //WHY IS LENGTH SO STRANGE?
		return response;
	} else {
		free (response);
		return NULL;
	}
}

char * toBin (int n, char ** buffer, int bits_per_component){
	*buffer = (char*)realloc (*buffer, sizeof(int)*8+1+1);
	*buffer[0]=0;
	for (int c = sizeof(int)*8-1; c >= 0; c--){
		if ((n>>c) & 1)
			strcat (*buffer, "1");
		else
			strcat (*buffer, "0");
		if (c==bits_per_component) strcat (*buffer, " ");
	}
	return *buffer;
}

// unfinished function
void dump_and_compare (FILE * one, FILE * two, const int bitsPerComponent){
	printf ("Comparing two files:\n");
	int char1 = getc (one);
	int char2 = getc (two);
			char * c1 = NULL; char * c2 = NULL;
	while (char1!=EOF && char2!=EOF){
				c1 = toBin (char1, &c1, bitsPerComponent);
				c2 = toBin (char2, &c2, bitsPerComponent);
		printf ("--------------------\n %c = %s\n %c = %s\n", char1, c1, char2, c2);

		char1 = getc (one); char2 = getc (two);
	}
			free (c1); free(c2);
}
