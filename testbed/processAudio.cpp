#include "sndfile.h"
#include <vector>
#include <iostream>
#define BLOCKSIZE 1024


int main()
{
    /* When open in read mode this info gets filled in.
    In write mode the caller must fill in the sample rate, channels and format.
    This examples uses the input info. */
    SF_INFO inFile_Inf;
    SF_INFO outFile_Inf;

    SNDFILE* inFile;
    SNDFILE* outFile;
    double* input = new double[BLOCKSIZE];
    double *output = new double[BLOCKSIZE];

    std::vector<float> inVec;
    std::vector<float> outVec;

    inFile = sf_open("media/test.wav", SFM_READ, &inFile_Inf);
    outFile_Inf = inFile_Inf;
    outFile = sf_open("media/output.wav", SFM_WRITE, &outFile_Inf);

    std::cout << "Frames = " << inFile_Inf.frames << std::endl;
    inVec.resize(inFile_Inf.frames, 0.f);
    outVec.resize(inFile_Inf.frames, 0.f);

    // items read can be less than blocksize when near the end
    while( int itemsRead = sf_readf_float(inFile, inVec.data(), inVec.size()) ){
            for(int i=0; i<itemsRead; ++i){
                outVec[i] = inVec[i];
            }
            sf_write_float(outFile, inVec.data(), itemsRead);
    };

    sf_close(inFile);
    sf_close(outFile);

    delete input;
    delete output;
	return 0;
}
