#ifndef FACTORY_AUDIOIO_HH
#define FACTORY_AUDIOIO_HH

/*
    This class will create AudioIO objects. It will also handle the destruction of the created objects.
    The AudioIO objects can still be created as instances using their constructors. However, using the Factory "createAudioIO" method
    is preferable as this will ensure only one instance is ever created. The destructors of AudioIO are also public so they can be called
    outside of the Factory. However, if the Factory goes out of scope it will automatically remove the AudioIO.


 */

#include "AudioIO.hh"
#include <string>
#include <iostream>
#include <map>

using namespace AudioInOut;

typedef AudioIO* (*t_ConstructAudioIO_Func)(std::string, int, int, int, int, const char*);

class Factory_AudioIO {
    private:
        static AudioIO* m_CreatedAudioIO;
        std::map<AudioIOType, t_ConstructAudioIO_Func> m_ConstructorList;
    public:
        Factory_AudioIO(){}
        AudioIO* createAudioIO(std::string audioIO, int chans, int sRate, int frameSize, int deviceIndex, const char* programName = "portaudio");
        void destroyAudioIO();
        ~Factory_AudioIO(); // Also calls destroyAudioIO().

};


#endif // FACTORY_AUDIOIO_HH
