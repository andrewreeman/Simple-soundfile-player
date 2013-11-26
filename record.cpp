// This uses portaudio to generate a sine wave using the default driver.

// TODO use vector instead of malloc for arrays.
//TODO Create a generic driver class that acts as a wrapper around portaudio. But also in the future has the possibility of
// wrapping other drivers.

#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include "portaudio.h"
#include <cmath>
#include <vector>

#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (512)
#define NUM_SECONDS     (3)
#define NUM_CHANNELS    (2)
#define PI 3.14159265

/* Select sample format. */
/* useful to typedef the sample incase we change it in the future */
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;

typedef struct
{
    /* The playCallback routine will read from the recorded samples and write to the audio driver.
    Frame index is the current index into recorded samples.
    */
    int          frameIndex;
    int          maxFrameIndex;
    SAMPLE      *recordedSamples;
}
paTestData;

void Pa_Cleanup(PaError err){
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
}

// wrapper for the audio driver. To be developed with RAII in mind.
class AudioDriver{
    private:
        int m_numChans;
        int m_sampleRate;
        int m_blockSize;
};

int main(void)
{
    PaStreamParameters outputParameters;
    PaStream* stream;
    PaError err = paNoError;
    paTestData data;
    int totalFrames;
    int numSamples;
    int numBytes;
    SAMPLE val;
    std::vector<SAMPLE> inputBuffer;



    data.maxFrameIndex = totalFrames = NUM_SECONDS * SAMPLE_RATE;
    data.frameIndex = 0;
    numSamples = totalFrames * NUM_CHANNELS;
    inputBuffer.resize(numSamples, 0.f);

    /* Data returns a pointer to the internal array. This should only be used if the vector is not later modified
    leading to the address changing.
    */
    data.recordedSamples = inputBuffer.data();

    // Fill the buffer. At the moment this is a sine wave. In the future it will be sound file data.
    for(int i=0; i<data.maxFrameIndex; i+=NUM_CHANNELS){
        data.recordedSamples[i] = sin(  440.f * PI * 2.f * (float)i / (float)SAMPLE_RATE) * 0.3;
        data.recordedSamples[i+1] = sin(  1.1 * 440.f * PI * 2.f * (float)i / (float)SAMPLE_RATE) * 0.3;

    }

    err = Pa_Initialize();


    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default output device.\n");
        Pa_Terminate();
        return 1;
    }
    outputParameters.channelCount = NUM_CHANNELS;                     /* stereo output */
    outputParameters.sampleFormat =  PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    // Pass the callback function and address of userdata.
    err = Pa_OpenStream(&stream, NULL, &outputParameters, SAMPLE_RATE,
              FRAMES_PER_BUFFER, paClipOff, NULL, &data );
    if( err != paNoError ){
        Pa_Cleanup(err);
        return 1;
    }

    err = Pa_StartStream( stream );
    if( err != paNoError ){
        Pa_Cleanup(err);
        return 1;
    }

    printf("Waiting for playback to finish.\n"); fflush(stdout);

    // if stream is active then continue streaming.
    //while( ( err = Pa_IsStreamActive( stream ) )) Pa_Sleep(100);


    for(int i=0; i<totalFrames/FRAMES_PER_BUFFER; ++i){
        err = Pa_WriteStream(stream, &data.recordedSamples[i+data.frameIndex], FRAMES_PER_BUFFER);
        if(err != paNoError){
            Pa_Cleanup(err);
            return 1;
        }
        data.frameIndex += (FRAMES_PER_BUFFER * NUM_CHANNELS) - 1;
    }
    err = Pa_StopStream(stream);
    if(err != paNoError){
            Pa_Cleanup(err);
            return 1;
    }

    if( err != paNoError ){
        Pa_Cleanup(err);
        return 1;
    }

    err = Pa_CloseStream( stream );
    if( err != paNoError ){
        Pa_Cleanup(err);
        return 1;
    }

    printf("Done.\n"); fflush(stdout);

    return 0;
}

