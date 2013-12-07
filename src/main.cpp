/*
Uses the AudioIO class to pass input buffers to PortAudio.
The AudioIO class is used as a wrapper for PortAudio leaving it possible to change to a different library/device.

*/

/* Select sample format. */
/* useful to typedef the sample incase we change it in the future */

#include <sndfile.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>

#include "../include/portaudio.h"
#include "../include/AudioIO.hh"
#include "../include/ioUtils.hh"
#include "../include/SoundFile.hh"

#define PI 3.14159265

int main(void)
{
        // I need to make this a singleton class as multiple instances of AudioIO is not desired.
    try{
        playSoundFile("media/test.wav");
        return 0;
    }
    catch(Pa_Exception paEx){
        std::cout << paEx.what() << std::endl;
        return 1;
    }
    catch(sndFile_Exception sndEx){
        std::cout << sndEx.what() << std::endl;
        return 1;
    }
    catch(...){
        std::cout << "Exception occurred" <<std::endl;
        return 1;
    }
}

