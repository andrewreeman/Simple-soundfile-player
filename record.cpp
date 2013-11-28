/*
Uses the AudioIO class to pass input buffers to PortAudio.
The AudioIO class is used as a wrapper for PortAudio leaving it possible to change to a different library/device.
This will play two sine waves at two different frequencies. One for the left channel and another for the right.
*/

#include <sndfile.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>

#include "portaudio.h"
#include "AudioIO.hh"

#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER (512)
#define NUM_SECONDS (3)
#define NUM_CHANNELS (2)
#define PI 3.14159265

void interleave(std::vector< std::vector<SAMPLE>* > inBuffers, std::vector<SAMPLE> &interleavedBuffer, int numChans){
    //PortAudio expects interleaved audio. This will take a vector of buffers for each channel and interleave them into the output buffer.

    // Compare input buffer sizes. Must match.
    for(int i = 0; i<numChans-1; ++i){
        int sizeA = inBuffers[i]->size();
        int sizeB = inBuffers[i+1]->size();
        if( sizeA != sizeB)
            throw std::exception();
    }
    int inputBufferSize = inBuffers[0]->size();
    int interleavedSize = interleavedBuffer.size();

    // Interleaved buffer must be able to fit all the input buffers!
    if(interleavedSize !=  inputBufferSize * numChans){
        throw std::exception();
    }
    for(int outputSamp=0, inputSamp=0; outputSamp<interleavedSize; outputSamp+=numChans, ++inputSamp){
        for(int chan=0; chan<numChans; chan++){
            interleavedBuffer[outputSamp+chan] = inBuffers[chan]->at(inputSamp);
        }
    }
}

int main(void)
{
    try{
        int totalFrames = SAMPLE_RATE * NUM_SECONDS;
        int inputBufferSize = totalFrames;
        int interleavedBufferSize = totalFrames * NUM_CHANNELS;
        int blockSize = FRAMES_PER_BUFFER * NUM_CHANNELS;

        std::vector<SAMPLE> inputBufferL;
        std::vector<SAMPLE> inputBufferR;
        std::vector<SAMPLE> inputBuffer_Interleaved;
        std::vector< std::vector<SAMPLE>* > v_inputBuffers(NUM_CHANNELS);

        // I need to make this a singleton class as multiple instances of AudioIO is not desired.
        AudioIO outputDevice(NUM_CHANNELS, SAMPLE_RATE, FRAMES_PER_BUFFER, "outputDevice");

        inputBufferL.resize(inputBufferSize, 0.f);
        inputBufferR.resize(inputBufferSize, 0.f);
        inputBuffer_Interleaved.resize(interleavedBufferSize, 0.f);

        // At the moment I am manually declaring the buffers. This needs to be modified to support multichannel.
        // The buffers should be declared as a vector of vector pointers from the start!
        v_inputBuffers[0] = &inputBufferL;
        v_inputBuffers[1] = &inputBufferR;

        // Fill buffers
        for(int i=0; i<inputBufferSize; ++i){
            inputBufferL[i] = sin(  440.f * PI * 2.f * (float)i / (float)SAMPLE_RATE) * 0.3;
        }
        for(int i=0; i<inputBufferSize; ++i){
            inputBufferR[i] = sin(  1.5 * 440.f * PI * 2.f * (float)i / (float)SAMPLE_RATE) * 0.3;
        }

        interleave(v_inputBuffers, inputBuffer_Interleaved, NUM_CHANNELS);
        // Audio is not prepared to be written to the soundcard

        outputDevice.start();
        for(int i=0; i<inputBuffer_Interleaved.size(); i+= blockSize){
            outputDevice.write(&inputBuffer_Interleaved[i]);
        }
        outputDevice.stop();

        return 0;
    }
    catch(std::exception e){
        // Instead of a standard exception I should throw a custom portaudio exception. Cleanup should not happen inside the AudioIO class.
        // It should be handled inside this catch.
        std::cout << "Exception occurred" <<std::endl;
        return 1;
    }
}

