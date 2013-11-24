all: record
record: record.o
	gcc -o record record.o libportaudio.a -lrt -lasound -ljack -lpthread 
record.o: record.c
	gcc -c record.c
clean: 
	rm -rf *.o record
	
