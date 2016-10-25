all : encoder decoder

encoder: encoder.o
	gcc -Wall encoder.o -o encoder

decoder: decoder.o
	gcc -Wall decoder.o -o decoder

encoder.o: encoder.c
	gcc -Wall -c encoder.c

decoder.o: decoder.c
	gcc -Wall -c decoder.c

clean:
	rm -f *.o encoder decoder output.c
