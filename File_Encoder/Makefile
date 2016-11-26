all : encoder decoder

debug: encoder decoder

encoder: encoder.o
	gcc -g -Wall -I/usr/include/python2.7 encoder.o -o encoder -lpython2.7

decoder: decoder.o
	gcc -g -Wall -I/usr/include/python2.7 decoder.o -o decoder -lpython2.7

encoder.o: encoder.c
	gcc -g -Wall -I/usr/include/python2.7 -c encoder.c -lpython2.7

decoder.o: decoder.c
	gcc -g -Wall -I/usr/include/python2.7 -c decoder.c -lpython2.7

clean:
	rm -f *.o encoder decoder output.c
