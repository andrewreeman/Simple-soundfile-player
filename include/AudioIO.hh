/*
    Wrapper for the audio IO used.
    AudioIO is an abstract class which provides the basic interface of a simple AudioIO object. PA_AudioIO uses Portaudio as the host api.
    Classes will be further derived from PA_AudioIO to provide functionality specific to certain audio apis.
*/

#ifndef __AudioIO__
#define __AudioIO__

#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
#include "portaudio.h"

#ifdef __linux__
    #include "pa_linux_alsa.h"
    #include "pa_jack.h"
#endif
#if defined(_WIN32) || defined(__CYGWIN__)
    #include "pa_asio.h"
    #include "pa_win_ds.h"
//    #include "pa_win_wmme.h"
#endif

#include "exceptions.hh"
#include "ioUtils.hh"
#include <map>


#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;

class Factory_AudioIO;

namespace AudioInOut{

enum class AudioIOType{
    PA_DEFAULT, PA_ALSA, PA_JACK, PA_OSS, PA_ASIO, PA_DS, PA_WMME, PA_MAX_TYPES
};

struct DevInfo{
    // Device information. 
    std::string devName;
    int numInputs;
    int numOutputs;
};

struct ApiInfo{
    //Api information
    std::string apiName;
    std::vector<DevInfo> devices;
};

struct AudioIO_Info{
    // General information about audio io settings.
    int chans, sampleRate, frames, deviceIndex;
    AudioIOType type;

    AudioIO_Info() : chans(1), sampleRate(44100), frames(512), deviceIndex(0), type(AudioIOType::PA_DEFAULT){}
    ~AudioIO_Info(){}
};

std::vector<ApiInfo> getHostApis();
AudioIOType stringToAudioIOType(std::string apiString);
AudioIOType intToAudioIOType(int api);

class AudioIO{
    // A general abstract wrapper for the audio io api.
    public:
        int m_numChans;
        int m_sampleRate;
        int m_frameSize; // The block size will be frameSize * numChans
        AudioIOType m_audioIOType;

        AudioIO(int chans, int sRate, int frameSize, AudioIOType type)
            : m_numChans(chans), m_sampleRate(sRate), m_frameSize(frameSize), m_audioIOType(type){}
        virtual ~AudioIO(){ std::cout << "AUDIOIO DESTROYED" << std::endl; }
        virtual void initialise() = 0;
        virtual void terminate() = 0;

        AudioIOType getAudioIOType(){ return m_audioIOType; }
        virtual DevInfo getDevInfo() = 0;
	
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

        virtual void enableRealTimeScheduling(bool enable){ std::cout << "Enable status: " << enable << " ignored. Real time scheduling not enabled by default." << std::endl; }
        virtual bool isRealTime(){ return 0; }
};

class PA_AudioIO : public AudioIO{
    // This child utilizes the portaudio api. The setDevice function must be implemented to derive from this class.
    protected:
        PaStreamParameters m_PaParams;
        PaStream* m_Stream;
        PaDeviceIndex m_DevInd;
    public:
        PA_AudioIO(int chans, int sRate, int frameSize, AudioIOType type, PaDeviceIndex index) : AudioIO(chans, sRate, frameSize, type), m_DevInd(index){}
        ~PA_AudioIO();
        virtual void initialise();
        virtual void terminate();

        DevInfo getDevInfo();

        virtual void write(SAMPLE *input);
        virtual void read(SAMPLE *output){ std::cout << "AudioIO::read(SAMPLE *output) is undefined. " << "Value: " << *output << " ignored. " << std::endl; }

        virtual void start();
        virtual void stop();

        virtual const char* getApi();
    protected:
        virtual void Pa_ErrorOccurred(PaError err);
	virtual PaDeviceIndex setDevice(int deviceIndex) = 0;
};

class PA_AudioIO_Default : public PA_AudioIO{
    public:
        PA_AudioIO_Default(int chans, int sRate, int frameSize, int deviceIndex) : PA_AudioIO(chans, sRate, frameSize, AudioIOType::PA_DEFAULT, deviceIndex){}
        ~PA_AudioIO_Default(){std::cout << "DESTROYING PA DEFAULT"; }
    protected:
        virtual PaDeviceIndex setDevice(int deviceIndex = 0);
};

#ifdef __linux__

class PA_AudioIO_ALSA : public PA_AudioIO{
    private:
        bool m_isRealTime;
    public:
        PA_AudioIO_ALSA(int chans, int sRate, int frameSize, int deviceIndex): PA_AudioIO(chans, sRate, frameSize, AudioIOType::PA_ALSA, deviceIndex),  m_isRealTime(0){}
        ~PA_AudioIO_ALSA(){ std::cout << "DESTORYING PA ALSA" << std::endl; }
        void enableRealTimeScheduling(bool enable);
        bool isRealTime(){ return m_isRealTime; }
    protected:
        virtual PaDeviceIndex setDevice(int deviceIndex);    
};

class PA_AudioIO_JACK : public PA_AudioIO{
    public:
        PA_AudioIO_JACK(int chans, int sRate, int frameSize, int deviceIndex) : PA_AudioIO(chans, sRate, frameSize, AudioIOType::PA_JACK, deviceIndex){}
        ~PA_AudioIO_JACK(){ std::cout << "DESTORYING PA JACK" << std::endl; }
        PaError setJackClientName(const char* programName);
    protected:
        virtual PaDeviceIndex setDevice(int deviceIndex);
};
#endif // end linux

#if defined(_WIN32) || defined(__CYGWIN__)
class PA_AudioIO_ASIO: public PA_AudioIO{
    public:
        PA_AudioIO_ASIO(int chans, int sRate, int frameSize, int deviceIndex) : PA_AudioIO(chans, sRate, frameSize, AudioIOType::PA_ASIO, deviceIndex){}
        ~PA_AudioIO_ASIO(){ std::cout << "DESTORYING PA ASIO" << std::endl; }
    protected:
        virtual PaDeviceIndex setDevice(int deviceIndex);
};

class PA_AudioIO_DS : public PA_AudioIO{
    public:
        PA_AudioIO_DS(int chans, int sRate, int frameSize, int deviceIndex) : PA_AudioIO(chans, sRate, frameSize, AudioIOType::PA_DS, deviceIndex){}
        ~PA_AudioIO_DS(){ std::cout << "DESTORYING PA DS" << std::endl; }
    protected:
        virtual PaDeviceIndex setDevice(int deviceIndex);
};

class PA_AudioIO_WMME : public PA_AudioIO{
    public:
        PA_AudioIO_WMME(int chans, int sRate, int frameSize, int deviceIndex) : PA_AudioIO(chans, sRate, frameSize, AudioIOType::PA_WMME, deviceIndex){}
        ~PA_AudioIO_WMME(){ std::cout << "DESTORYING PA WMME" << std::endl; }
    protected:
        virtual PaDeviceIndex setDevice(int deviceIndex);
};

#endif // end windows

}; // end namespace
#endif 


