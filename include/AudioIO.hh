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
#include "exceptions.hh"
#include "ioUtils.hh"

#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;

struct ApiInfo{
    std::string apiName;
    std::vector<std::string> devices;
};

class AudioIO{
    protected:
        int m_numChans;
        int m_sampleRate;
        int m_frameSize; // The block size will be frameSize * numChans

        /* This will contain information about all the apis the host api can find.
        If AudioIO is using a system api directly then it will return a vector of one element */
        std::vector<ApiInfo> m_vApiInf;
    public:
        AudioIO(int chans, int sRate, int frameSize)
            : m_numChans(chans), m_sampleRate(sRate), m_frameSize(frameSize){}
        ~AudioIO(){}

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
    protected:
        // This will usually be run at construction to get all the available api data.
        virtual void fillHostApiInfo() = 0;
};

class PA_AudioIO : public AudioIO {
    protected:
        PaStreamParameters m_PaParams;
        PaStream* m_Stream;
    public:
        PA_AudioIO(int chans, int sRate, int frameSize);
        ~PA_AudioIO();

        virtual void write(SAMPLE *input);
        virtual void read(SAMPLE *output){}

        virtual void start();
        virtual void stop();
    protected:
        virtual void Pa_ErrorOccurred(PaError err);
        virtual void fillHostApiInfo();
};

#endif
