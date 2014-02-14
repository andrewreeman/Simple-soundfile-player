#include "../include/AudioIO.hh"

PA_AudioIO::PA_AudioIO(int chans, int sRate, int frameSize)
    : AudioIO(chans, sRate, frameSize){
    /* This will initialize Portaudio. Get the default device then open a stream.
     * This is potentially unsafe as this is a constructor that can fail. Another way would be to have a simple constructor
     * which must always be followed by an init() function. However this leads to all functions having to check
     * if the object has been initialised.
     * I prefer the potentially unsafe constructor as if this fails it will be clean
    */
    try{
        Pa_Initialize();
        fillHostApiInfo();
        m_PaParams.device = Pa_GetDefaultOutputDevice();
        if(m_PaParams.device == paNoDevice) throw Pa_NoDeviceException();


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
    catch(Pa_NoDeviceException paEx){
        std::cerr << "An error occurred while trying to open the audio device" << std::endl;
        std::cerr << "Error message: " << paEx.what() << std::endl;
        Pa_Terminate();
        throw; // rethrow exception
    }
    catch(Pa_StreamException paEx){
        if( Pa_IsStreamActive(m_Stream) ) Pa_StopStream(m_Stream);
        std::cerr << "An error occured while using the portaudio stream" << std::endl;
        std::cerr << "Error message: " << paEx.what() << std::endl;
        Pa_Terminate();
        throw;
    }
    catch(Pa_Exception paEx){
        if( Pa_IsStreamActive(m_Stream) ) Pa_StopStream(m_Stream);
        std::cerr << "An error occurred while initializing the Audio IO" << std::endl;
        std::cerr << "Error message: " << paEx.what() << std::endl;
        Pa_Terminate();
        throw;
    }
    catch(...){
        std::cerr << "Exception occurred during AudioIO object construction." <<std::endl;
    }
}

PA_AudioIO::~PA_AudioIO(){
    if( Pa_IsStreamActive(m_Stream) ) Pa_StopStream(m_Stream);
    Pa_Terminate();
    std::cout << "HOWDY WORLD" << std::endl;
}

void PA_AudioIO::fillHostApiInfo(){
    PaHostApiIndex apiCount = Pa_GetHostApiCount();
    PaDeviceIndex devCount = 0;
    PaDeviceIndex devNum = 0;
    std::vector<std::string> devs;
    const PaHostApiInfo* p_ApiInf;
    const PaDeviceInfo* p_DevInf;
    m_vApiInf.resize(apiCount);

    for(int api=0; api<apiCount; ++api){
        p_ApiInf = Pa_GetHostApiInfo(api);
        m_vApiInf[api].apiName = p_ApiInf->name;
        devCount = p_ApiInf->deviceCount;
        devs.resize(devCount);
        for(int dev=0; dev<devCount; ++dev){
            devNum = Pa_HostApiDeviceIndexToDeviceIndex(api, dev);
            p_DevInf = Pa_GetDeviceInfo(devNum);
            devs[dev] = p_DevInf->name;
            m_vApiInf[api].devices = devs;
        }
    }
}



void PA_AudioIO::write(SAMPLE* input){
    //Expects interleaved audio. Frame size is NOT the block size.
    PaError err = Pa_WriteStream(m_Stream, input, m_frameSize);
    if(err != paNoError)
        Pa_ErrorOccurred(err);
}

void PA_AudioIO::Pa_ErrorOccurred(PaError err){
        if( Pa_IsStreamActive(m_Stream) ) Pa_StopStream(m_Stream);
        fprintf( stderr, "An error occured while using the portaudio stream\n" );
        fprintf( stderr, "Error number: %d\n", err );
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
        Pa_Terminate();
        throw Pa_StreamException();
}

void PA_AudioIO::start(){
    PaError err = Pa_StartStream(m_Stream);
    if(err != paNoError)
        Pa_ErrorOccurred(err);
}

void PA_AudioIO::stop(){
    // Stop the stream
    PaError err = paNoError;
    if( Pa_IsStreamActive(m_Stream) ) err = Pa_StopStream(m_Stream);
    if(err!=paNoError) Pa_ErrorOccurred(err);
    err = Pa_CloseStream(m_Stream);
    if(err!=paNoError) Pa_ErrorOccurred(err);
}

void PA_AudioIO_ALSA::enableRealTimeScheduling(bool enable){
    PaAlsa_EnableRealtimeScheduling(m_Stream, enable);
    m_isRealTime = enable;
    if(m_isRealTime){
        std::cout << "Real time scheduling is enabled." << std::endl;
    }
    else{
        std::cout << "Real time scheduling is disabled." << std::endl;
    }

}

