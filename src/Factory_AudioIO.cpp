#include "../include/Factory_AudioIO.hh"
#include <typeinfo>
#include <string>

using namespace AudioInOut;

// implement the makeAudioIO for the Worker classes
AudioIO* Worker_PaDefault::makeAudioIO(AudioIO_Info aio_inf){
    AudioIO* p_aio = new PA_AudioIO_Default(aio_inf.chans, aio_inf.sampleRate, aio_inf.frames, aio_inf.deviceIndex);
    p_aio->initialise();
    return p_aio;
}
#ifdef __linux__
AudioIO* Worker_PaAlsa::makeAudioIO(AudioIO_Info aio_inf){
    AudioIO* p_aio = new PA_AudioIO_ALSA(aio_inf.chans, aio_inf.sampleRate, aio_inf.frames, aio_inf.deviceIndex);
    p_aio->initialise();
    return p_aio;
}
AudioIO* Worker_PaJack::makeAudioIO(AudioIO_Info aio_inf){
    AudioIO* p_aio = new PA_AudioIO_JACK(aio_inf.chans, aio_inf.sampleRate, aio_inf.frames, aio_inf.deviceIndex);
    p_aio->initialise();
    return p_aio;
}
#endif
#if defined(_WIN32) || defined(__CYGWIN__)
AudioIO* Worker_PaAsio::makeAudioIO(AudioIO_Info aio_inf){
    AudioIO* p_aio = new PA_AudioIO_ASIO(aio_inf.chans, aio_inf.sampleRate, aio_inf.frames, aio_inf.deviceIndex);
    p_aio->initialise();
    return p_aio;
}

AudioIO* Worker_PaDs::makeAudioIO(AudioIO_Info aio_inf){
    AudioIO* p_aio = new PA_AudioIO_DS(aio_inf.chans, aio_inf.sampleRate, aio_inf.frames, aio_inf.deviceIndex);
    p_aio->initialise();
    return p_aio;
}

AudioIO* Worker_PaWmme::makeAudioIO(AudioIO_Info aio_inf){
    AudioIO* p_aio = new PA_AudioIO_WMME(aio_inf.chans, aio_inf.sampleRate, aio_inf.frames, aio_inf.deviceIndex);
    p_aio->initialise();
    return p_aio;
}
#endif

AudioIO* Factory_AudioIO::m_CreatedAudioIO = 0; // how many audioIO's are active? At the moment this mechanism works with only 1 being allowed existence

Factory_AudioIO::Factory_AudioIO(){
    // Fill the constructor list. This will select the correct Worker to create the correct AudioIO object
    m_ConstructorList[AudioIOType::PA_DEFAULT] = new Worker_PaDefault();
#ifdef __linux__
    m_ConstructorList[AudioIOType::PA_ALSA] = new Worker_PaAlsa();
    m_ConstructorList[AudioIOType::PA_JACK] = new Worker_PaJack();
#endif
#if defined(_WIN32) || defined(__CYGWIN__)
    m_ConstructorList[AudioIOType::PA_ASIO] = new Worker_PaAsio();
    m_ConstructorList[AudioIOType::PA_DS] = new Worker_PaDs();
    m_ConstructorList[AudioIOType::PA_WMME] = new Worker_PaWmme();
#endif
}

AudioIO* Factory_AudioIO::createAudioIO(AudioIOType AudioIO, int chans, int sRate, int frameSize, int deviceIndex){
    if(m_CreatedAudioIO) throw F_InstanceAlreadyExistsException();
    try{
        AudioIO_Info inf;
        inf.chans = chans;
        inf.sampleRate = sRate;
        inf.frames = frameSize;
        inf.deviceIndex = deviceIndex;
        m_CreatedAudioIO = (m_ConstructorList[AudioIO])->makeAudioIO(inf);
        return m_CreatedAudioIO;
    }
    catch(...){
        if(m_CreatedAudioIO) delete m_CreatedAudioIO;
        m_CreatedAudioIO = 0;
        throw;
    }
    return nullptr;
}


void Factory_AudioIO::destroyAudioIO(){
    if(m_CreatedAudioIO){
        m_CreatedAudioIO->terminate();
        delete m_CreatedAudioIO;
        std::cout << "Factory destroyed AudioIO" << std::endl;
    }
    m_CreatedAudioIO = 0;
}

Factory_AudioIO::~Factory_AudioIO(){
    std::map<AudioIOType, Worker_AudioIO*>::iterator iter;
    for(iter = m_ConstructorList.begin(); iter!= m_ConstructorList.end(); ++iter){
        delete iter->second;
    }
    destroyAudioIO();
}
