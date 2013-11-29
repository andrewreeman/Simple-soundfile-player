all: main
main: main.o 
	g++ -o playSoundFile main.o AudioIO.o ioUtils.o libs/libportaudio.a -lsndfile -lrt -lasound -ljack -lpthread
	rm -rf *.o
main.o: src/main.cpp src/AudioIO.cpp src/ioUtils.cpp
	g++ -c src/ioUtils.cpp
	g++ -c src/AudioIO.cpp
	g++ -c src/main.cpp
clean: 
	rm -rf *.o record *~
	
