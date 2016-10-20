# Makefile made by Anton -- the only person who does not know how they work
test:
	g++ encodePNG.cpp -o run
	./run test.png husky.png
	compare -compose src test.png got.png
clean:
	rm run packed.png got.png diff.png
	
#CC=g++
#CFLAGS=-c -Wall
#LDFAGS=# none
#SOURCES=encodePNG.c
#OBJECTS=#(SOURCES:.cpp=.o)
#EXECUTABLE=run

#all: run

#run: encodePNG.c
#	$(CC) encodePNG.c -o run
#all: $(SOURCES) $(EXECUTABLE)

#$(EXECUTABLE): $(OBJETCS)
#	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

#.cpp.o:
#	$(CC) ${CFLAGS) $< -o $@
