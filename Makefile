CC=g++
CFLAGS = -Wall
OBJDIR = src
OBJECTS = $(addprefix $(OBJDIR)/, main.o AudioIO.o ioUtils.o)
INCDIR = include
LIBDIR = libs
LIBS = $(addprefix $(LIBDIR)/, libportaudio.a) -lsndfile -lrt -lasound -ljack -lpthread

all: main

main: $(OBJECTS)
	$(CC) -o playSoundFile $(OBJECTS) $(LIBS)
	rm -rf *.o

$(objects): $(INCDIR)/ioUtils.hh

# using implicit rules
main.o AudioIO.o: $(INCDIR)/AudioIO.hh

.PHONY: clean

clean: 
	rm -rf $(OBJECTS) playSoundFile *~ $(OBJDIR)/*~ $(INCDIR)/*~
	
