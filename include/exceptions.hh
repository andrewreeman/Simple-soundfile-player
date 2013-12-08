/*
    Contains custom exceptions. At the moment the errors are still unclear.
*/

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>

class Pa_Exception : public std::exception{
    public:
        virtual const char* what() const throw() { return "Portaudio exception."; }
};

class Pa_NoDeviceException : public Pa_Exception{
    public:
    virtual const char* what() const throw() { return "Portaudio exception: Could not open audio device."; }

};

class Pa_StreamException : public Pa_Exception{
    public:
    virtual const char* what() const throw() { return "Portaudio exception: Error during stream."; }
};

class sndFile_Exception : public std::exception{
    public:
        virtual const char* what() const throw () { return "Libsndfile exception"; }
};

#endif
