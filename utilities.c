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


int main (){
	if (littleEndian())
		write (1, "Sysytem is little endian.\n", 26);
	else	write (1, "Sysytem is big endian.\n", 23);
	return 0;
}
