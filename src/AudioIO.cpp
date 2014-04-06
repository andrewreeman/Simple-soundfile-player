#include "../include/AudioIO.hh"
#include <cstring>


namespace AudioInOut{


std::vector<ApiInfo> getHostApis(){

    if( Pa_Initialize() != paNoError ) throw std::runtime_error("Portaudio could not be initialized during 'getHostApis'.");
    PaHostApiIndex apiCount = Pa_GetHostApiCount();
    PaDeviceIndex devCount = 0;
    PaDeviceIndex devNum = 0;
    std::vector<DevInfo> devInf;
    const PaHostApiInfo* p_ApiInf;
    const PaDeviceInfo* p_DevInf;

    std::vector<ApiInfo> vApiInf(apiCount);

    for(int api=0; api<apiCount; ++api){
        p_ApiInf = Pa_GetHostApiInfo(api);
        vApiInf[api].apiName = p_ApiInf->name;
        devCount = p_ApiInf->deviceCount;
        devInf.resize(devCount);
        for(int dev=0; dev<devCount; ++dev){

            devNum = Pa_HostApiDeviceIndexToDeviceIndex(api, dev);
            p_DevInf = Pa_GetDeviceInfo(devNum);
            devInf[dev].devName = p_DevInf->name;
            devInf[dev].numInputs = p_DevInf->maxInputChannels;
            devInf[dev].numOutputs= p_DevInf->maxOutputChannels;
            vApiInf[api].devices = devInf;
		}
    }
    if( Pa_Terminate() != paNoError ) throw std::runtime_error("Error terminating portaudio during 'getHostApis'.");
    return vApiInf;
}

std::map<int, AudioIOType> initPaType_To_AudioIO_Map(){
    std::map<int, AudioIOType> v_map;
    v_map[paDirectSound] = AudioIOType::PA_DS;
    v_map[paMME] = AudioIOType::PA_WMME;
    v_map[paALSA] = AudioIOType::PA_ALSA;
    v_map[paASIO] = AudioIOType::PA_ASIO;
    v_map[paJACK] = AudioIOType::PA_JACK;
    v_map[paOSS] = AudioIOType::PA_OSS;
    return v_map;
}

AudioIOType intToAudioIOType(int api){
    const PaHostApiInfo* hostApiInf = NULL;
    std::map<int, AudioIOType> v_map = initPaType_To_AudioIO_Map();
    PaHostApiTypeId type;

    if( Pa_Initialize() != paNoError ) throw std::runtime_error("Portaudio could not be initialized during 'intToAudioIOType'.");
    hostApiInf = Pa_GetHostApiInfo(api);
    if(hostApiInf == NULL){
        if( Pa_Terminate() != paNoError ) throw std::runtime_error("Error terminating portaudio during 'intToAudioIOType'.");
        throw std::runtime_error("An error occurred during Pa_GetHostApiInfo in 'intToAudioIOType'.");
     }
    type = hostApiInf->type;
    if( Pa_Terminate() != paNoError ) throw std::runtime_error("Error terminating portaudio during 'intToAudioIOType'.");
    return v_map[type];

}

PA_AudioIO::~PA_AudioIO(){
    if( Pa_IsStreamActive(m_Stream) ) Pa_StopStream(m_Stream);
    Pa_Terminate();
    std::cout << "HOWDY WORLD" << std::endl;
}

void PA_AudioIO::initialise(){
    try{
        Pa_Initialize();
        m_PaParams.device = setDevice(m_DevInd); //Pa_GetDefaultOutputDevice();
        if(m_PaParams.device == paNoDevice) throw Pa_NoDeviceException();


        m_PaParams.channelCount = m_numChans;
        m_PaParams.sampleFormat = PA_SAMPLE_TYPE;
        m_PaParams.suggestedLatency = Pa_GetDeviceInfo( m_PaParams.device )->defaultLowOutputLatency;
        m_PaParams.hostApiSpecificStreamInfo = NULL; // Takes a void pointer. Can be used for passing other data to stream.

        // Open the stream. Ready for starting
        PaError err = Pa_OpenStream(&m_Stream, NULL, &m_PaParams, m_sampleRate,
                  m_frameSize, paClipOff, NULL, NULL );
        if(err != paNoError)
            Pa_ErrorOccurred(err);
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

const char* PA_AudioIO::getApi(){
    PaDeviceIndex DevInd = m_PaParams.device;
    const PaDeviceInfo* DevInf = Pa_GetDeviceInfo(DevInd);
    PaHostApiIndex ApiInd = DevInf->hostApi;
    const PaHostApiInfo* ApiInf = Pa_GetHostApiInfo(ApiInd);
    return ApiInf->name;
}

void PA_AudioIO::write(SAMPLE* input){
    //Expects interleaved audio. Frame size is NOT the block size.
    PaError err = Pa_WriteStream(m_Stream, input, m_frameSize);
    if(err != paNoError)
        Pa_ErrorOccurred(err);
}

void PA_AudioIO::terminate(){
        if( Pa_IsStreamActive(m_Stream) ) Pa_StopStream(m_Stream);
        Pa_Terminate();
        std::cout << "PA terminated" << std::endl;
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

DevInfo PA_AudioIO::getDevInfo(){
    PaDeviceIndex paDevInd = m_PaParams.device ;
    const PaDeviceInfo* paInf = Pa_GetDeviceInfo(paDevInd);
    DevInfo devInf;
    devInf.devName = paInf->name;
    devInf.numInputs = paInf->maxInputChannels;
    devInf.numOutputs = paInf->maxOutputChannels;
    return devInf;
}

PaDeviceIndex PA_AudioIO_Default::setDevice(int deviceIndex){
    return Pa_GetDefaultOutputDevice();
}

#ifdef __linux__

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


PaDeviceIndex PA_AudioIO_ALSA::setDevice(int deviceIndex = 0){
    PaHostApiIndex alsaInd = Pa_HostApiTypeIdToHostApiIndex(paALSA);
    const PaHostApiInfo* apiInf = Pa_GetHostApiInfo(alsaInd);

    if(deviceIndex > apiInf->deviceCount ||  deviceIndex < 0){
        throw Pa_DeviceIndexNotFoundException();
    }

    return Pa_HostApiDeviceIndexToDeviceIndex(alsaInd, deviceIndex);
}

PaDeviceIndex PA_AudioIO_JACK::setDevice(int deviceIndex){
    PaHostApiIndex jackInd = Pa_HostApiTypeIdToHostApiIndex(paJACK);
    const PaHostApiInfo* apiInf = nullptr;

    apiInf = Pa_GetHostApiInfo(jackInd);
    if(!apiInf) throw Pa_NoApiException();
    if(deviceIndex > apiInf->deviceCount ||  deviceIndex < 0){
        throw Pa_DeviceIndexNotFoundException();
    }

    return Pa_HostApiDeviceIndexToDeviceIndex(jackInd, deviceIndex);
}

PaError PA_AudioIO_JACK::setJackClientName(const char *programName){
    return PaJack_SetClientName(programName);
}
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
PaDeviceIndex PA_AudioIO_ASIO::setDevice(int deviceIndex){
    PaHostApiIndex asioInd = Pa_HostApiTypeIdToHostApiIndex(paASIO);
    const PaHostApiInfo* apiInf = nullptr;

    apiInf = Pa_GetHostApiInfo(asioInd);
    if(!apiInf) throw Pa_NoApiException();
    if(deviceIndex > apiInf->deviceCount ||  deviceIndex < 0){
        throw Pa_DeviceIndexNotFoundException();
    }

    std::cout << "Warning: ASIO blocking is currently unimplemented in portaudio." << std::endl;
    return Pa_HostApiDeviceIndexToDeviceIndex(asioInd, deviceIndex);
}

PaDeviceIndex PA_AudioIO_DS::setDevice(int deviceIndex){
    PaHostApiIndex dsInd = Pa_HostApiTypeIdToHostApiIndex(paDirectSound);
    const PaHostApiInfo* apiInf = nullptr;

    apiInf = Pa_GetHostApiInfo(dsInd);
    if(!apiInf) throw Pa_NoApiException();
    if(deviceIndex > apiInf->deviceCount ||  deviceIndex < 0){
        throw Pa_DeviceIndexNotFoundException();
    }
    return Pa_HostApiDeviceIndexToDeviceIndex(dsInd, deviceIndex);

}

PaDeviceIndex PA_AudioIO_WMME::setDevice(int deviceIndex){
    PaHostApiIndex mmeInd = Pa_HostApiTypeIdToHostApiIndex(paMME);
    const PaHostApiInfo* apiInf = nullptr;

    apiInf = Pa_GetHostApiInfo(mmeInd);
    if(!apiInf) throw Pa_NoApiException();
    if(deviceIndex > apiInf->deviceCount ||  deviceIndex < 0){
        throw Pa_DeviceIndexNotFoundException();
    }
    return Pa_HostApiDeviceIndexToDeviceIndex(mmeInd, deviceIndex); // how do I find the first output device ?
}


#endif

}
