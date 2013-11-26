// This uses portaudio to generate a sine wave using the default driver.

//TODO Add comments
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
#define NUM_CHANNELS    (1)
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

/* This routine will be called by the PortAudio engine when audio is needed.
** It may be called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int playCallback( const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData ){

    /* Inputbuffer is the input from the driver/soundcard. Outbufer is to the output.
     This callback will write the recorded samples to the outputbuffer in blocks of "framesPerBuffer".
     In the final block when the number of frames left to be written is less than framesPerBuffer it will write
     these final frames then perform zero padding for the remaining output frames. */

    paTestData *data = (paTestData*)userData;
    // This sets the read pointer to the current frame index. Taking into acount the interleaving of channels
    SAMPLE *rptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
    SAMPLE *wptr = (SAMPLE*)outputBuffer;
    unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;
    int i; // declared here to save multiple declarations

    if( framesLeft > framesPerBuffer )
    {
        for( i=0; i<framesPerBuffer; i++ )
        {
            // This is inadequate for operations with over 2 channels. An wrapping for loop is prefered
            *wptr++ = *rptr++;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
        }
        data->frameIndex += framesPerBuffer;
        return paContinue;
    }
    else
    {
        // final run. Write final frames
        for( i=0; i<framesLeft; i++ )
        {
            *wptr++ = *rptr++;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
        }
        // Then zero pad remaining output frames
        for(i = framesLeft ; i<framesPerBuffer; i++ )
        {
            *wptr++ = 0;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = 0;  /* right */
        }
        // This will bring the frameindex to equal maxFrameIndex.
        data->frameIndex += framesLeft;
        return paComplete;
    }
}

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

    data.maxFrameIndex = totalFrames = NUM_SECONDS * SAMPLE_RATE;
    data.frameIndex = 0;
    numSamples = totalFrames * NUM_CHANNELS;
    numBytes = numSamples * sizeof(SAMPLE);
    // easier to do new?? Or in fact use a vector for cleanup on destruction
    data.recordedSamples = (SAMPLE *) malloc( numBytes );

    // Fill the buffer. At the moment this is a sine wave. In the future it will be sound file data.
    for(int i=0; i<data.maxFrameIndex; ++i){
        data.recordedSamples[i] = sin(  440.f * PI * 2.f * (float)i / (float)SAMPLE_RATE);
    }

    err = Pa_Initialize();


    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default output device.\n");
        Pa_Terminate();
        free(data.recordedSamples);
        return 1;
    }
    outputParameters.channelCount = NUM_CHANNELS;                     /* stereo output */
    outputParameters.sampleFormat =  PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    // Pass the callback function and address of userdata.
    err = Pa_OpenStream(&stream, NULL, &outputParameters, SAMPLE_RATE,
              FRAMES_PER_BUFFER, paClipOff, playCallback, &data );
    if( err != paNoError ){
        Pa_Cleanup(err);
        free(data.recordedSamples);
        return 1;
    }

    err = Pa_StartStream( stream );
    if( err != paNoError ){
        Pa_Cleanup(err);
        free(data.recordedSamples);
        return 1;
    }

    printf("Waiting for playback to finish.\n"); fflush(stdout);

    // if stream is active then continue streaming.
    while( ( err = Pa_IsStreamActive( stream ) )) Pa_Sleep(100);
    if( err != paNoError ){
        Pa_Cleanup(err);
        free(data.recordedSamples);
        return 1;
    }

    err = Pa_CloseStream( stream );
    if( err != paNoError ){
        Pa_Cleanup(err);
        free(data.recordedSamples);
        return 1;
    }

    printf("Done.\n"); fflush(stdout);

    return 0;
}

