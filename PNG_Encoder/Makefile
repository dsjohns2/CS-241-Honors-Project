# CS241 Honors (CS296-41) Project: Steganography Toolset
# Makefile for the main (UI), encodePNG, and encrypt

all: run png txt

png: source_PNG/test.png source_PNG/husky.png run
	./run source_PNG/test.png source_PNG/husky.png packed.png got.png
	compare -compose src source_PNG/husky.png packed.png diff.png
	diff source_PNG/test.png got.png

txt: test.png husky.png run
	./run source_PNG/test.txt source_PNG/husky.png packed.png got.txt
	compare -compose src source_PNG/husky.png packed.png diff.png
	diff test.txt got.txt # > diff.txt
#	cat diff.txt

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
	rm -f *.o run packed.png got.png diff.png oneTimePad.bin got.txt diff.txt

data:
	mkdir source_PNG
# large Husky
	wget -O ./source_PNG/husky_big.png http://f.fwallpapers.com/images/husky.png
	wget -O ./source_PNG/test.png https://drive.google.com/file/d/0B0T_W5_kbwv0VjBPOGg1VlI3RUE/view?usp=sharing
