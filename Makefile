CC=g++
CFLAGS = -Wall
objects = src/main.o src/AudioIO.o src/ioUtils.o

all: main

main: $(objects)
	$(CC) -o playSoundFile $(objects) libs/libportaudio.a -lsndfile -lrt -lasound -ljack -lpthread
	rm -rf *.o

$(objects): src/ioUtils.cpp

# using implicit rules
main.o AudioIO.o: src/AudioIO.cpp 

.PHONY: clean

clean: 
	rm -rf $(objects) playSoundFile *~ src/*~
	
