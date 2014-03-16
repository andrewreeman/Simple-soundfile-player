SYS := $(shell uname)
CC=g++
CFLAGS = -Wall -std=c++0x # -Werror 
OBJDIR = src
SOURCES = $(addprefix $(OBJDIR)/, main.cpp AudioIO.cpp ioUtils.cpp SoundFile.cpp Factory_AudioIO.cpp)
OBJECTS = $(addprefix $(OBJDIR)/, main.o AudioIO.o ioUtils.o SoundFile.o Factory_AudioIO.o)
INCDIR = include
DEPS = $(addprefix $(INCDIR)/, AudioIO.hh ioUtils.hh exceptions.hh portaudio.h sndfile.h sndfile.hh SoundFile.hh Factory_AudioIO.hh)
LIBDIR = libs
ifeq ($(SYS), Linux)
	LIBS = $(addprefix $(LIBDIR)/, libportaudio.a) -lsndfile -lrt -lasound -ljack -lpthread
else
	LIBS = $(addprefix $(LIBDIR)/, portaudio_x86.dll) -lsndfile -lrt -lpthread 
endif

EXECUTABLE = simpleSF_Player



all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
ifeq ($(SYS), Linux)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LIBS)
else
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LIBS)
	cp $(LIBDIR)/portaudio_x86.dll  .
endif

.cpp.o:
	g++ $(CFLAGS) -c $< -o $@

#src/main.o: src/main.cpp 
#	g++ -c $< -o $@

#src/AudioIO.o: src/AudioIO.cpp include/AudioIO.hh
#	g++ -c $< -o $@

#src/ioUtils.o: src/ioUtils.cpp include/ioUtils.hh
#	g++ -c $< -o $@

#src/SoundFile.o: src/SoundFile.cpp include/SoundFile.hh
#	g++ -c $< -o $@

#src/Factory_AudioIO.o: src/Factory_AudioIO.cpp include/Factory_AudioIO.hh
#	g++ -c $< -o $@


# using implicit rules
#main.o AudioIO.o: $(INCDIR)/AudioIO.hh

.PHONY: clean

clean: 
	rm -rf $(OBJECTS) $(EXECUTABLE) *~ $(OBJDIR)/*~ $(INCDIR)/*~ $(EXECUTABLE).exe portaudio_x86.dll



