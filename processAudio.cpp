#include "sndfile.h"
#define BLOCKSIZE 1024


int main()
{
    /* When open in read mode this info gets filled in.
    In write mode the caller must fill in the sample rate, channels and format.
    This examples uses the input info. */
    SF_INFO inFile_Inf;
    SNDFILE* inFile;
    SNDFILE* outFile;
    double* input = new double[BLOCKSIZE];
    double *output = new double[BLOCKSIZE];

    inFile = sf_open("media/test.wav", SFM_READ, &inFile_Inf);
    outFile = sf_open("media/output.wav", SFM_WRITE, &inFile_Inf);

    // items read can be less than blocksize when near the end
    while( int itemsRead = sf_readf_double(inFile, input, BLOCKSIZE) ){
            for(int i=0; i<itemsRead; ++i){
                output[i] = input[i];
            }
            sf_writef_double(outFile, output, itemsRead);
    };

    sf_close(inFile);
    sf_close(outFile);

    delete input;
    delete output;
	return 0;
}
