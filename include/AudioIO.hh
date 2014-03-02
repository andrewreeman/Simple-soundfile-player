/*
    Wrapper for the audio IO method used.
    The playing of a soundfile should be a method used in the SoundFile class.
    The SoundFile class will be passed a pointer to the AudioIO object.

    AudioIO is an abstract class which provides the basic interface of a simple AudioIO object. PA_AudioIO uses Portaudio as the host api.
    Classes will be further derived from PA_AudioIO to provide functionality specific to certain audio apis.

*/

#ifndef __AudioIO__
#define __AudioIO__

#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include "portaudio.h"
#include "pa_linux_alsa.h"
#include "pa_jack.h"

#include "exceptions.hh"
#include "ioUtils.hh"


#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;

struct ApiInfo{
    std::string apiName;
    std::vector<std::string> devices;
};

enum class AudioIOType{
    PA_DEFAULT, PA_ALSA, PA_JACK
};

class AudioIO{

    friend class Factory_AudioIO;

    protected:
        int m_numChans;
        int m_sampleRate;
        int m_frameSize; // The block size will be frameSize * numChans
        AudioIOType m_audioIOType;

        /* This will contain information about all the apis the host api can find.
        If AudioIO is using a system api directly then it will return a vector of one element */
        std::vector<ApiInfo> m_vApiInf;

    protected:
        AudioIO(int chans, int sRate, int frameSize, AudioIOType type)
            : m_numChans(chans), m_sampleRate(sRate), m_frameSize(frameSize), m_audioIOType(type){}
        ~AudioIO(){ std::cout << "AUDIOIO DESTROYED" << std::endl; }
        virtual void initialise() = 0;
        virtual void terminate() = 0;

    public:
        AudioIOType getAudioIOType(){ return m_audioIOType; }
        virtual std::vector<ApiInfo> getHostApis()const{ return m_vApiInf; }

        virtual void write(SAMPLE* input) = 0;
        virtual void read(SAMPLE* output) = 0;

        void setChans(int chans);
        int getChans()const{return m_numChans;}


        void setSampleRate(int sampleRate);
        int getSampleRate()const{return m_sampleRate;}

        void setFrameSize(int frameSize);
        int getFrameSize()const{return m_frameSize;}

        virtual void start() = 0;
        virtual void stop() = 0;

        virtual void enableRealTimeScheduling(bool enable){ std::cout << "Real time scheduling not enabled by default." << std::endl; }
        virtual bool isRealTime(){ return 0; }
    protected:
        // This will usually be run at construction to get all the available api data.
        virtual void fillHostApiInfo() = 0;
};

class PA_AudioIO : public AudioIO{

    friend class Factory_AudioIO;

    protected:
        PaStreamParameters m_PaParams;
        PaStream* m_Stream;
        PaDeviceIndex m_DevInd;
    protected:
        PA_AudioIO(int chans, int sRate, int frameSize, AudioIOType type, PaDeviceIndex index) : AudioIO(chans, sRate, frameSize, type){}
        ~PA_AudioIO();
        virtual void initialise() = 0;
        virtual void terminate();
    public:
        virtual void write(SAMPLE *input);
        virtual void read(SAMPLE *output){}

        virtual void start();
        virtual void stop();

        virtual const char* getApi();
        virtual const char* getDevice();
    protected:
        virtual void Pa_ErrorOccurred(PaError err);
        virtual void fillHostApiInfo();
        virtual PaDeviceIndex setDevice(int deviceIndex) = 0;
};

class PA_AudioIO_Default : public PA_AudioIO{

    friend class Factory_AudioIO;

    protected:
        PA_AudioIO_Default(int chans, int sRate, int frameSize, int deviceIndex);
        ~PA_AudioIO_Default(){std::cout << "DESTROYING PA DEFAULT"; }
        virtual PaDeviceIndex setDevice(int deviceIndex = 0);
        virtual void initialise();
};

class PA_AudioIO_ALSA : public PA_AudioIO{

    friend class Factory_AudioIO;

    private:
        bool m_isRealTime;
    protected:
        PA_AudioIO_ALSA(int chans, int sRate, int frameSize, int deviceIndex);
        ~PA_AudioIO_ALSA(){ std::cout << "DESTORYING PA ALSA" << std::endl; }
        virtual PaDeviceIndex setDevice(int deviceIndex);
        virtual void initialise();
    public:
        void enableRealTimeScheduling(bool enable);
        bool isRealTime(){ return m_isRealTime; }

};

class PA_AudioIO_JACK : public PA_AudioIO{

    friend class Factory_AudioIO;

    protected:
        PA_AudioIO_JACK(int chans, int sRate, int frameSize, int deviceIndex);
        ~PA_AudioIO_JACK(){ std::cout << "DESTORYING PA JACK" << std::endl; }
        virtual PaDeviceIndex setDevice(int deviceIndex);
        virtual void initialise();
    public:
        PaError setJackClientName(const char* programName);

};

#endif


