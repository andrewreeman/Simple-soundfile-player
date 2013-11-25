all: processAudio record
record: record.o
	g++ -o record record.o libportaudio.a -lrt -lasound -ljack -lpthread 
record.o: record.cpp
	g++ -c record.cpp
processAudio: processAudio.o
	g++ -o processAudio processAudio.o -lsndfile
processAudio.o: processAudio.cpp
	g++ -c processAudio.cpp
clean: 
	rm -rf *.o record processAudio
	
