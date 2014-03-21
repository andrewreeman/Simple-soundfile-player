#include "../include/Factory_AudioIO.hh"
#include <typeinfo>
#include <string>

using namespace AudioInOut;


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


AudioIO* Factory_AudioIO::m_CreatedAudioIO = 0;

/*
AudioIO* Factory_AudioIO::makePaDefault(int chans, int sRate, int frameSize, int deviceIndex, const char *programName){
    return new PA_AudioIO_Default(chans, sRate, frameSize, deviceIndex);
}

#ifdef __linux__
AudioIO* Factory_AudioIO::makePaAlsa(int chans, int sRate, int frameSize, int deviceIndex, const char *programName){
    return new PA_AudioIO_ALSA(chans, sRate, frameSize, deviceIndex);
}

AudioIO* Factory_AudioIO::makePaJack(int chans, int sRate, int frameSize, int deviceIndex, const char *programName){
    AudioIO* p_AudioIO = new PA_AudioIO_JACK(chans, sRate, frameSize, deviceIndex);
    dynamic_cast<PA_AudioIO_JACK*>(p_AudioIO)->setJackClientName(programName);
    return p_AudioIO;
}
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
AudioIO* Factory_AudioIO::makePaAsio(int chans, int sRate, int frameSize, int deviceIndex, const char *programName){
    return new PA_AudioIO_ASIO(chans, sRate, frameSize, deviceIndex);
}

AudioIO* Factory_AudioIO::makePaDs(int chans, int sRate, int frameSize, int deviceIndex, const char *programName){
    return new PA_AudioIO_DS(chans, sRate, frameSize, deviceIndex);
}

AudioIO* Factory_AudioIO::makePaMME(int chans, int sRate, int frameSize, int deviceIndex, const char *programName){
    return new PA_AudioIO_WMME(chans, sRate, frameSize, deviceIndex);
}
#endif

*/


//USE A CONSTRUCTAUDIOIO FUNCTION THAT WILL SELECT THE CORRECT CONSTRUCTOR. AND A POPULATE CONSTRUCTOR LIST TOADD CONSTRUCTORS TO

Factory_AudioIO::Factory_AudioIO(){
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

/*
#ifdef __linux__
    m_ConstructorList.insert(
        std::pair<AudioIOType, t_ConstructAudioIO_Func>(AudioIOType::PA_ALSA, &Factory_AudioIO::makePaAlsa)
    );
    m_ConstructorList.insert(
        std::pair<AudioIOType, t_ConstructAudioIO_Func>(AudioIOType::PA_JACK, &Factory_AudioIO::makePaJack)
    );
#endif
#if defined(_WIN32) || defined(__CYGWIN__)
    m_ConstructorList.insert(
        std::pair<AudioIOType, t_ConstructAudioIO_Func>(AudioIOType::PA_ASIO, &Factory_AudioIO::makePaAsio)
    );
    m_ConstructorList.insert(
        std::pair<AudioIOType, t_ConstructAudioIO_Func>(AudioIOType::PA_DS, &Factory_AudioIO::makePaDs)
    );
    m_ConstructorList.insert(
        std::pair<AudioIOType, t_ConstructAudioIO_Func>(AudioIOType::PA_MME, &Factory_AudioIO::makePaMME)
    );

#endif
*/
}

AudioIO* Factory_AudioIO::createAudioIO(std::string AudioIO, int chans, int sRate, int frameSize, int deviceIndex, const char* programName){
    if(m_CreatedAudioIO) throw F_InstanceAlreadyExistsException();
    try{
/*        if(AudioIO == "portaudio_default") m_CreatedAudioIO = new PA_AudioIO_Default(chans, sRate, frameSize, deviceIndex);
#ifdef __linux__
        else if(AudioIO == "portaudio_jack"){
            m_CreatedAudioIO = new PA_AudioIO_JACK(chans, sRate, frameSize, deviceIndex);
            dynamic_cast<PA_AudioIO_JACK*>(m_CreatedAudioIO)->setJackClientName(programName);
        }
        else if(AudioIO == "portaudio_alsa") m_CreatedAudioIO = new PA_AudioIO_ALSA(chans, sRate, frameSize, deviceIndex);
#endif
#if defined(_WIN32) || defined(__CYGWIN__)
        else if(AudioIO == "portaudio_asio") m_CreatedAudioIO = new PA_AudioIO_ASIO(chans, sRate, frameSize, deviceIndex);
        else if(AudioIO == "portaudio_ds") m_CreatedAudioIO = new PA_AudioIO_DS(chans, sRate, frameSize, deviceIndex);
        else if(AudioIO == "portaudio_mme") m_CreatedAudioIO = new PA_AudioIO_WMME(chans, sRate, frameSize, deviceIndex);
#endif
        else throw F_NotValidDeviceNameException();
        m_CreatedAudioIO->initialise();
        return m_CreatedAudioIO;
*/
        AudioIO_Info inf;
        inf.chans = chans;
        inf.sampleRate = sRate;
        inf.frames = frameSize;
        inf.deviceIndex = 0;
        TODO: AudioIOType from argument after being checked for correctness. Next: do checks
        m_CreatedAudioIO = (m_ConstructorList[AudioIOType::PA_DEFAULT])->makeAudioIO(inf);
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





