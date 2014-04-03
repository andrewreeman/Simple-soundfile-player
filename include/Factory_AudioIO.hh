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

typedef AudioIO* (Factory_AudioIO::* t_ConstructAudioIO_Func)(int chans, int sRate, int frameSize, int deviceIndex, const char* programName);

class Worker_AudioIO;

class Factory_AudioIO {
    private:
        static AudioIO* m_CreatedAudioIO;
        std::map<AudioIOType, Worker_AudioIO*> m_ConstructorList;

        AudioIO* makePaDefault(int chans, int sRate, int frameSize, int deviceIndex, const char* programName);

    public:
        Factory_AudioIO();
        AudioIO* createAudioIO(AudioIOType audioIO, int chans, int sRate, int frameSize, int deviceIndex, const char* programName = "portaudio");
        void destroyAudioIO();
        ~Factory_AudioIO(); // Also calls destroyAudioIO().

};

class Worker_AudioIO{
    public:
        virtual AudioIO* makeAudioIO(AudioIO_Info) = 0;
};

class Worker_PaDefault : public Worker_AudioIO{
    public:
        virtual AudioIO* makeAudioIO(AudioIO_Info);
};
#ifdef __linux__
class Worker_PaAlsa : public Worker_AudioIO{
    public:
        virtual AudioIO* makeAudioIO(AudioIO_Info);
};

class Worker_PaJack : public Worker_AudioIO{
    public:
        virtual AudioIO* makeAudioIO(AudioIO_Info);
};
#endif
#if defined(_WIN32) || defined(__CYGWIN__)
class Worker_PaAsio : public Worker_AudioIO{
    public:
        virtual AudioIO* makeAudioIO(AudioIO_Info);
};

class Worker_PaDs : public Worker_AudioIO{
    public:
        virtual AudioIO* makeAudioIO(AudioIO_Info);
};

class Worker_PaWmme : public Worker_AudioIO{
    public:
        virtual AudioIO* makeAudioIO(AudioIO_Info);
};
#endif



#endif // FACTORY_AUDIOIO_HH
