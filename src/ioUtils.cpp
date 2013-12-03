#include "../include/ioUtils.hh"

void interleave(std::vector< std::vector<float>* > inBuffers, std::vector<float> &interleavedBuffer, int numChans){
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



