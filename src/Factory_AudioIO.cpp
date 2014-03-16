#include "../include/Factory_AudioIO.hh"
#include <typeinfo>
#include <string>

using namespace AudioInOut;

AudioIO* Factory_AudioIO::m_CreatedAudioIO = 0;

AudioIO* Factory_AudioIO::createAudioIO(std::string AudioIO, int chans, int sRate, int frameSize, int deviceIndex, const char* programName){
    if(m_CreatedAudioIO) throw F_InstanceAlreadyExistsException();
    try{
        if(AudioIO == "portaudio_default") m_CreatedAudioIO = new PA_AudioIO_Default(chans, sRate, frameSize, deviceIndex);
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
    destroyAudioIO();
}





