SYS := $(shell uname)
CC=g++
CFLAGS = -Wall
OBJDIR = src
OBJECTS = $(addprefix $(OBJDIR)/, main.o AudioIO.o ioUtils.o SoundFile.o)
SOURCES = $(addprefix $(OBJDIR)/, main.cpp AudioIO.cpp ioUtils.cpp SoundFile.cpp)
INCDIR = include
DEPS = $(addprefix $(INCDIR)/, AudioIO.hh ioUtils.hh exceptions.hh portaudio.h sndfile.h sndfile.hh SoundFile.hh)
LIBDIR = libs
ifeq ($(SYS), Linux)
	LIBS = $(addprefix $(LIBDIR)/, libportaudio.a) -lsndfile -lrt -lasound -ljack -lpthread
else
	LIBS = $(addprefix $(LIBDIR)/, portaudio_x86.dll) -lsndfile -lrt -lpthread 
endif


#$(OBJECTS): $(SOURCES) $(DEPS)
#	$(CC) -c -o $@ $< $(CFLAGS)

all: main

main: $(OBJECTS) $(SOURCES) $(DEPS)
ifeq ($(SYS), Linux)
	$(CC) -o playSoundFile $(OBJECTS) $(LIBS)
else
	$(CC) -o playSoundFile $(OBJECTS) $(LIBS)
	cp $(LIBDIR)/portaudio_x86.dll  .
endif
	



# using implicit rules
#main.o AudioIO.o: $(INCDIR)/AudioIO.hh

.PHONY: clean

clean: 
ifeq ($(SYS), Linux)
	rm -rf $(OBJECTS) playSoundFile *~ $(OBJDIR)/*~ $(INCDIR)/*~
else
	rm -rf $(OBJECTS) playSoundFile *~ $(OBJDIR)/*~ $(INCDIR)/*~ portaudio_x86.dll	 
endif
