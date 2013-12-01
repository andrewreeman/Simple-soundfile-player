CC=g++
CFLAGS = -Wall
objects = main.o AudioIO.o ioUtils.o

all: main
main: $(objects)
	$(CC) -o playSoundFile $(objects) libs/libportaudio.a -lsndfile -lrt -lasound -ljack -lpthread
	rm -rf *.o
main.o: src/main.cpp src/AudioIO.cpp src/ioUtils.cpp
	$(CC) -c src/ioUtils.cpp
	$(CC) -c src/AudioIO.cpp
	$(CC) -c src/main.cpp

.PHONY: clean
clean: 
	rm -rf *.o playSoundFile *~
	
