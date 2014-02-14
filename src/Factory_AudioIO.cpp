#include "../include/Factory_AudioIO.hh"

AudioIO* Factory_AudioIO::m_CreatedAudioIO = 0;



AudioIO* Factory_AudioIO::createAudioIO(std::string AudioIOType, int chans, int sRate, int frameSize){
    if(m_CreatedAudioIO) throw F_InstanceAlreadyExistsException();

    if(AudioIOType == "portaudio_alsa"){
        try{
            m_CreatedAudioIO = new PA_AudioIO_ALSA(chans, sRate, frameSize);
            return m_CreatedAudioIO;
        }
        catch(...){
            if(m_CreatedAudioIO) delete m_CreatedAudioIO;
            m_CreatedAudioIO = 0;
            throw;
        }
    }
    else{
        throw F_NotValidDeviceNameException();
    }
}


void Factory_AudioIO::destroyAudioIO(){
    if(m_CreatedAudioIO){
        delete m_CreatedAudioIO;
        std::cout << "Factory destroyed AudioIO" << std::endl;
    }
    m_CreatedAudioIO = 0;
}

Factory_AudioIO::~Factory_AudioIO(){
    destroyAudioIO();
}




