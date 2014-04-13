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

class Pa_DeviceIndexNotFoundException : public Pa_Exception{
    public:
        virtual const char* what() const throw() { return "Portaudio exception: An error occurred while trying to open the audio device";}
};

class Pa_NoApiException : public Pa_Exception{
    public:
        virtual const char* what() const throw() {return "Portaudio exception: Api was not found";}
};

class sndFile_Exception : public std::exception{
    public:
        virtual const char* what() const throw () { return "Libsndfile exception"; }
};

class sndFileStd_Exception : public sndFile_Exception{
	public:
        virtual const char* what() const throw () { return "Libsndfile exception: error opening soundfile"; }
};

class Factory_AudioIOException : public std::exception{
    public:
       virtual const char* what() const throw() { return "Factory AudioIO exception"; }

};

class F_NotValidDeviceNameException : public Factory_AudioIOException{
    public:
        virtual const char* what() const throw() {return "Factory AudioIO exception: Device name is not valid"; }
};

class F_InstanceAlreadyExistsException : public Factory_AudioIOException{
    public:
        virtual const char* what() const throw() {return "Factory AudioIO exception: An instance of an AudioIO object already exists."; }
};

#endif
