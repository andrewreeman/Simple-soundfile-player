SYS := $(shell uname)
CC=g++
CFLAGS = -Wall
OBJDIR = src
OBJECTS = $(addprefix $(OBJDIR)/, main.o AudioIO.o ioUtils.o SoundFile.o)
INCDIR = include
LIBDIR = libs
ifeq ($(SYS), Linux)
	LIBS = $(addprefix $(LIBDIR)/, libportaudio.a) -lsndfile -lrt -lasound -ljack -lpthread
else
	LIBS = $(addprefix $(LIBDIR)/, portaudio_x86.dll) -lsndfile -lrt -lpthread 
endif



all: main

main: $(OBJECTS)
ifeq ($(SYS), Linux)
	$(CC) -o playSoundFile $(OBJECTS) $(LIBS)
else
	$(CC) -o playSoundFile $(OBJECTS) $(LIBS)
	cp $(LIBDIR)/portaudio_x86.dll  .
endif
	

$(objects): $(INCDIR)/ioUtils.hh

# using implicit rules
main.o AudioIO.o: $(INCDIR)/AudioIO.hh

.PHONY: clean

clean: 
ifeq ($(SYS), Linux)
	rm -rf $(OBJECTS) playSoundFile *~ $(OBJDIR)/*~ $(INCDIR)/*~
else
	rm -rf $(OBJECTS) playSoundFile *~ $(OBJDIR)/*~ $(INCDIR)/*~ portaudio_x86.dll	 
endif
