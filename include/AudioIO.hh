/*
    Wrapper for the audio IO method used. This should eventually be a singleton and
    provide various methods for AudioIO instead of just portaudio.
    The playing of a soundfile should be a method used in the SoundFile class.
    The SoundFile class will be passed a pointer to the AudioIO object.
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
    private:

        int m_numChans;
        int m_sampleRate;
        int m_frameSize; // The block size will be frameSize * numChans
        std::string m_DeviceName;
        PaStreamParameters m_PaParams;
        PaStream* m_Stream;

        void Pa_ErrorOccurred(PaError err);

        //const std::vector<std::string> getHostApis();
        //const std::vector<std::string> getAvailableDevices(std::string hostApi);
        void fillHostApis();
        std::vector<ApiInfo> m_vApiInf;


    public:
        AudioIO(int chans, int sRate, int frameSize, std::string device);
        ~AudioIO();

        //const std::vector< std::vector<std::string> > getAllApiWithDevices();
        std::vector<ApiInfo> getHostApis()const{ return m_vApiInf; }

        void write(SAMPLE* input);
        void read(SAMPLE* output);

        void setChans(int chans);
        int getChans();

        void setSampleRate(int sampleRate);
        int getSampleRate();

        void setFrameSize(int frameSize);
        int getFrameSize();

        void setDevice(std::string);
        //void setDriver(int driver);
        std::string getDevice();

		std::vector<std::string> getAvailableDevices();

        void start();
        void stop();
};

#endif
