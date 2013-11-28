/*
Uses the AudioIO class to pass input buffers to PortAudio.
The AudioIO class is used as a wrapper for PortAudio leaving it possible to change to a different library/device.
This will play two sine waves at two different frequencies. One for the left channel and another for the right.
*/

// TODO better exception handling. Custom exception for PA failure. And sound file failure.

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
#define NUM_CHANNELS (1)
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


        int totalFrames = SAMPLE_RATE * NUM_SECONDS;
        int inputBufferSize = totalFrames;
        int interleavedBufferSize = totalFrames * NUM_CHANNELS;
        int blockSize = FRAMES_PER_BUFFER * NUM_CHANNELS;

        SF_INFO inFile_Inf;
        SNDFILE* inFile;

        std::vector<SAMPLE> inputBufferL;
        std::vector<SAMPLE> inputBufferR;
        std::vector<SAMPLE> inputBuffer_Interleaved;
        std::vector< std::vector<SAMPLE>* > v_inputBuffers(NUM_CHANNELS);

        // I need to make this a singleton class as multiple instances of AudioIO is not desired.
        try{
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

            inFile = sf_open("media/test.wav", SFM_READ, &inFile_Inf);
            std::vector<SAMPLE> sfBuffer(inFile_Inf.frames);

            sf_readf_float(inFile, sfBuffer.data(), sfBuffer.size());

            interleave(v_inputBuffers, inputBuffer_Interleaved, NUM_CHANNELS);
            // Audio is not prepared to be written to the soundcard

            outputDevice.start();

            for(int i=0; i<sfBuffer.size(); i+= blockSize){
                outputDevice.write(&sfBuffer[i]);
            }

            outputDevice.stop();

            sf_close(inFile);
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

