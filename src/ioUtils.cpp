#include "../include/ioUtils.hh"

void interleave(std::vector< std::vector<float>* > inBuffers, std::vector<float> &interleavedBuffer, int numChans){

    //PortAudio expects interleaved audio. This will take a vector of buffers for each channel and interleave them into the output buffer.

    // Compare input buffer sizes. Must match.
#ifndef NDEBUG

    for(int i = 0; i<numChans-1; ++i){
        int sizeA = inBuffers[i]->size();
        int sizeB = inBuffers[i+1]->size();
        if( sizeA != sizeB)
            assert("Interleaving error: Vector sizes do not match.");
    }
#endif

    int interleavedSize = interleavedBuffer.size();

#ifndef NDEBUG
    // Interleaved buffer must be able to fit all the input buffers!
    int inputBufferSize = inBuffers[0]->size();
    if(interleavedSize !=  inputBufferSize * numChans){
        assert("Interleaving error: Interleaved buffer is too small to fit input buffers.");
    }
#endif

    for(int outputSamp=0, inputSamp=0; outputSamp<interleavedSize; outputSamp+=numChans, ++inputSamp){
        for(int chan=0; chan<numChans; chan++){
            interleavedBuffer[outputSamp+chan] = inBuffers[chan]->at(inputSamp);
        }
    }
}



