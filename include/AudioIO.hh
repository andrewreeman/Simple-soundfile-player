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
#include "portaudio.h"
#include "exceptions.hh"
#include "ioUtils.hh"

#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;

class AudioIO{
    private:
        int m_numChans;
        int m_sampleRate;
        int m_frameSize; // The block size will be frameSize * numChans
        std::string m_DriverName;
        PaStreamParameters m_PaParams;
        PaStream* m_Stream;

    void Pa_ErrorOccurred(PaError err);

    public:
        AudioIO(int chans, int sRate, int frameSize, std::string driver);
        ~AudioIO();

        void write(SAMPLE* input);
        void read(SAMPLE* output);

        void setChans(int chans);
        int getChans();

        void setSampleRate(int sampleRate);
        int getSampleRate();

        void setFrameSize(int frameSize);
        int getFrameSize();

        void setDriver(std::string);
        //void setDriver(int driver);
        std::string getDriver();

        void start();
        void stop();
};

#endif
