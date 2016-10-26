# Makefile made by Anton -- the only person who does not know how they work
all: diff.png

diff.png: test.png husky.png run
	./run test.png husky.png packed.png got.png
	diff test.png got.png
	compare -compose src test.png got.png diff.png

run: main.c encodePNG.cpp
	g++ main.c -o run

#run: main.o encodePNG.o encrypt.o
#	gcc -Wall main.o encodePNG.o encrypt.o -o run

main.o: main.c
	gcc -Wall -c main.c

encodePNG.o: lodepng.cpp lodepng.h encodePNG.cpp
	gcc -Wall -c encodePNG.cpp

encrypt.o: encrypt.c
	gcc -Wall -c encrupt.c

clean:
	rm *.o run packed.png got.png diff.png oneTimePad.bin
