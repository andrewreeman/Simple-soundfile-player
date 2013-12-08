/*
    At the moment this is home to the playSoundFile function. This might be expanded into
    a larger soundfile class that simply encapsulates the libsndfile functions.
    Also contained here is a "playsine" function used for testing.
*/

#ifndef SOUNDFILE_H
#define SOUNDFILE_H

#ifdef _WIN32
    #include "sndfile.hh"
#else
    #include <sndfile.hh>
#endif

#include "AudioIO.hh"
#include <vector>
#include <math.h>
#include <string>

#define FRAMES_PER_BUFFER (512)
#define TWO_PI 6.28315

void playSoundFile(const char* soundfile);

void playSine();

#endif
