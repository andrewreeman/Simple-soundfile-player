/*
    Utilities for audio io
*/

#ifndef IOUTILS_H
#define IOUTILS_H

//If NotDebug is NOT defined (i.e....if in debug mode!)
#ifndef NDEBUG
    #include <cassert>
#endif

#include <vector>
#include <iostream>

void interleave(std::vector< std::vector<float>* > inBuffers, std::vector<float> &interleavedBuffer, int numChans);
std::vector<float> monoToStereoInterleaved(std::vector<float> monoInput_StereoOut);

#endif
