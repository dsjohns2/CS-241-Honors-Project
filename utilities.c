// CS  Honors Project (Fall 2016)
// Some Utility functions that might be handy
// Ask Anton Bershanskiy

#include <unistd.h>

//Endiedness of the system: Bigendien vs littleendian
// 1 if little endian, 0 if big endian
char littleEndian(){
	int a = 1;
	return *(char*)&a;
}


// uname -m (-a)
/*void systemType(){
	pid_t child = fork();
	if (child) execlp ("uname", "uname", "-m", NULL);//-a for more info
	else waitpid (child, NULL, 0);
}*/


/*
int main (){
	if (littleEndian())
		write (1, "Sysytem is little endian.\n", 26);
	else	write (1, "Sysytem is big endian.\n", 23);
	return 0;
}*/

char * toBin (int n, char * buffer, int bits_per_component){
	buffer = (char*)realloc (buffer, sizeof(int)*8+1+1);
	buffer[0]=0;
	for (int c = sizeof(int)*8-1; c >= 0; c--){
		if ((n>>c) & 1)
			strcat (buffer, "1");
		else
			strcat (buffer, "0");
		if (c==bits_per_component) strcat (buffer, " ");
	}
	return buffer;
}
