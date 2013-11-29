/*
Uses the AudioIO class to pass input buffers to PortAudio.
The AudioIO class is used as a wrapper for PortAudio leaving it possible to change to a different library/device.
This will play two sine waves at two different frequencies. One for the left channel and another for the right.
*/

/* Select sample format. */
/* useful to typedef the sample incase we change it in the future */

#include <sndfile.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>

#include "portaudio.h"
#include "AudioIO.hh"
#include "ioUtils.hh"

#define FRAMES_PER_BUFFER (512)
#define PI 3.14159265

void playSoundFile(const char* soundfile){
    /* Uses sndfile to open a sound file and read into a buffer.
    Then uses portaudio to play this file.
    */
    SF_INFO inFile_Inf;
    SNDFILE* inFile;
    inFile = sf_open(soundfile, SFM_READ, &inFile_Inf);
    int blockSize = FRAMES_PER_BUFFER * inFile_Inf.channels;
    int interleavedBufferSize = inFile_Inf.frames * inFile_Inf.channels;
    std::vector<SAMPLE> inputBuffer_Interleaved(interleavedBufferSize);
    AudioIO outputDevice(inFile_Inf.channels, inFile_Inf.samplerate, FRAMES_PER_BUFFER, "outputDevice");

    sf_readf_float(inFile, inputBuffer_Interleaved.data(), interleavedBufferSize);
    outputDevice.start();
    for(int i=0; i<interleavedBufferSize; i+= blockSize )
        outputDevice.write(&inputBuffer_Interleaved[i]);

    outputDevice.stop();
    sf_close(inFile);
}

int main(void)
{
        // I need to make this a singleton class as multiple instances of AudioIO is not desired.
    try{
        playSoundFile("media/test.wav");
        return 0;
    }
    catch(Pa_Exception paEx){
        std::cout << paEx.what() << std::endl;
        return 1;
    }
    catch(sndFile_Exception sndEx){
        std::cout << sndEx.what() << std::endl;
        return 1;
    }
    catch(...){
        // Instead of a standard exception I should throw a custom portaudio exception. Cleanup should not happen inside the AudioIO class.
        // It should be handled inside this catch.
        // Will not stop soundfile as soundfile does not exist in this scope
        std::cout << "Exception occurred" <<std::endl;
        return 1;
    }
}

