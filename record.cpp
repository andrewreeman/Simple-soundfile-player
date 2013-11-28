// This uses portaudio to generate a sine wave using the default driver.

// TODO use vector instead of malloc for arrays.
//TODO Create a generic driver class that acts as a wrapper around portaudio. But also in the future has the possibility of
// wrapping other drivers.


#include <sndfile.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>

#include "portaudio.h"
#include "AudioIO.hh"

#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (512)
#define NUM_SECONDS     (3)
#define NUM_CHANNELS    (2)
#define PI 3.14159265

int main(void)
{
    try{
        int totalFrames;
        int numSamples;
        int numBytes;
        SAMPLE val;
        std::vector<SAMPLE> inputBuffer;

        AudioIO test(NUM_CHANNELS, SAMPLE_RATE, FRAMES_PER_BUFFER, "test");
        totalFrames = SAMPLE_RATE * NUM_SECONDS;
        numSamples = totalFrames * NUM_CHANNELS;
        inputBuffer.resize(numSamples, 0.f);

        for(int i=0; i<totalFrames; i+=NUM_CHANNELS){
            inputBuffer[i] = sin(  440.f * PI * 2.f * (float)i / (float)SAMPLE_RATE) * 0.3;
            inputBuffer[i+1] = sin(  1.5 * 440.f * PI * 2.f * (float)i / (float)SAMPLE_RATE) * 0.3;

            inputBuffer[i] *= sin(PI*2.f * (float)i / (float)SAMPLE_RATE);
            inputBuffer[i+1] *= sin(PI*2.f * (float)i / (float)SAMPLE_RATE);
        }

        test.start();

        int blockSize = FRAMES_PER_BUFFER * NUM_CHANNELS;
        for(int i=0; i<numSamples; i+= blockSize){
            test.write(&inputBuffer[i]);
        }

        test.stop();

        return 0;
    }
    catch(std::exception e){
        std::cout << "Exception occurred" <<std::endl;
        return 1;
    }
}

