#ifndef IOUTILS_H
#define IOUTILS_H

#include <vector>

void interleave(std::vector< std::vector<float>* > inBuffers, std::vector<float> &interleavedBuffer, int numChans);

#endif
