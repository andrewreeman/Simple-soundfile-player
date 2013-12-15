#include "../include/AudioIO.hh"


AudioIO::AudioIO(int chans, int sRate, int frameSize, std::string device)
    :m_numChans(chans), m_sampleRate(sRate), m_frameSize(frameSize), m_DeviceName(device){

    // Initialize and open device
    Pa_Initialize();
    m_PaParams.device = Pa_GetDefaultOutputDevice();
    if (m_PaParams.device == paNoDevice) {
        fprintf(stderr,"Error: No default output device.\n");
        Pa_Terminate();
        throw Pa_NoDeviceException();
    }

    // Set paramaters
    m_PaParams.channelCount = m_numChans;
    m_PaParams.sampleFormat = PA_SAMPLE_TYPE;
    m_PaParams.suggestedLatency = Pa_GetDeviceInfo( m_PaParams.device )->defaultLowOutputLatency;
    m_PaParams.hostApiSpecificStreamInfo = NULL; // Takes a void pointer. Can be used for passing other data to stream.

    // Open the stream. Read for starting
    PaError err = Pa_OpenStream(&m_Stream, NULL, &m_PaParams, m_sampleRate,
              m_frameSize, paClipOff, NULL, NULL );
    if(err != paNoError)
        Pa_ErrorOccurred(err);
}
AudioIO::~AudioIO(){
    // Close if stream is open and then terminate portaudio.
    if( Pa_IsStreamActive(m_Stream) ) Pa_StopStream(m_Stream);
    Pa_Terminate();
}
void AudioIO::Pa_ErrorOccurred(PaError err){
        if( Pa_IsStreamActive(m_Stream) ) Pa_StopStream(m_Stream);
        fprintf( stderr, "An error occured while using the portaudio stream\n" );
        fprintf( stderr, "Error number: %d\n", err );
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
        Pa_Terminate();
        throw Pa_StreamException();
}
void AudioIO::start(){
    PaError err = Pa_StartStream(m_Stream);
    if(err != paNoError)
        Pa_ErrorOccurred(err);
}
void AudioIO::write(SAMPLE* input){
    //Expects interleaved audio. Frame size is NOT the block size.
    PaError err = Pa_WriteStream(m_Stream, input, m_frameSize);
    if(err != paNoError)
        Pa_ErrorOccurred(err);
}
void AudioIO::stop(){
    // Stop the stream
    PaError err = paNoError;
    if( Pa_IsStreamActive(m_Stream) ) err = Pa_StopStream(m_Stream);
    if(err!=paNoError) Pa_ErrorOccurred(err);
    err = Pa_CloseStream(m_Stream);
    if(err!=paNoError) Pa_ErrorOccurred(err);
}
