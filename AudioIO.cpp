#include "AudioIO.hh"


AudioIO::AudioIO(int chans, int sRate, int frameSize, std::string driver)
    :m_numChans(chans), m_sampleRate(sRate), m_frameSize(frameSize){
    Pa_Initialize();
    m_PaParams.device = Pa_GetDefaultOutputDevice();
    if (m_PaParams.device == paNoDevice) {
        fprintf(stderr,"Error: No default output device.\n");
        Pa_Terminate();
        throw Pa_Exception();
    }

    m_PaParams.channelCount = m_numChans;
    m_PaParams.sampleFormat = PA_SAMPLE_TYPE;
    m_PaParams.suggestedLatency = Pa_GetDeviceInfo( m_PaParams.device )->defaultLowOutputLatency;
    m_PaParams.hostApiSpecificStreamInfo = NULL;


    PaError err = Pa_OpenStream(&m_Stream, NULL, &m_PaParams, m_sampleRate,
              m_frameSize, paClipOff, NULL, NULL );
    if(err != paNoError)
        Pa_Cleanup(err);
}
AudioIO::~AudioIO(){
    if( Pa_IsStreamActive(m_Stream) ) Pa_StopStream(m_Stream);
    Pa_Terminate();
    std::cout << "PA DESTROYED" << std::endl;
}
void AudioIO::Pa_Cleanup(PaError err){
        if( Pa_IsStreamActive(m_Stream) ) Pa_StopStream(m_Stream);
        fprintf( stderr, "An error occured while using the portaudio stream\n" );
        fprintf( stderr, "Error number: %d\n", err );
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
        Pa_Terminate();
        throw Pa_Exception();
}
void AudioIO::start(){
    PaError err = Pa_StartStream(m_Stream);
    if(err != paNoError)
        Pa_Cleanup(err);
}
void AudioIO::write(SAMPLE* input){
    //Expects interleaved audio
    PaError err = Pa_WriteStream(m_Stream, input, m_frameSize);
    if(err != paNoError)
        Pa_Cleanup(err);
}
void AudioIO::stop(){
    PaError err = Pa_StopStream(m_Stream);
    if(err!=paNoError) Pa_Cleanup(err);
    err = Pa_CloseStream(m_Stream);
    if(err!=paNoError) Pa_Cleanup(err);
}
