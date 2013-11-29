#include <exception>

class Pa_Exception : public std::exception{
    public:
        virtual const char* what() const throw() { return "Portaudio exception"; }
};

class sndFile_Exception : public std::exception{
    public:
        virtual const char* what() const throw () { return "Libsndfile exception"; }
};




