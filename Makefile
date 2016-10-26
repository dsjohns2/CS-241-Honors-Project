# Makefile made by Anton -- the only person who does not know how they work
all: diff.png

diff.png: test.png husky.png run
	./run test.png husky.png packed.png got.png
#	diff test.png got.png
	compare -compose src husky.png packed.png diff.png

run: main.o encodePNG.o encrypt.o utilities.o
	g++ -Wall encodePNG.o encrypt.o utilities.o main.o -o run

main.o: main.c
	g++ -Wall -c main.c

encodePNG.o: lodepng.cpp lodepng.h encodePNG.cpp
	g++ -Wall -c encodePNG.cpp

encrypt.o: encrypt.c
	g++ -Wall -c encrypt.c

utilities.o: utilities.c
	g++ -Wall -c utilities.c

clean:
	rm -f *.o run packed.png got.png diff.png oneTimePad.bin
