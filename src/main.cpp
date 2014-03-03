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
#include "../include/Factory_AudioIO.hh"
#include "../include/ioUtils.hh"
#include "../include/SoundFile.hh"

#define PI 3.14159265

const char* PROGRAM_NAME = "Simple Soundfile Player";

void displayDrivers(){
    std::vector<ApiInfo> v_apiInfo = AudioInOut::getHostApis();
    for(unsigned api=0; api<v_apiInfo.size(); ++api){
        std::cout << "Api " << api << " : " << v_apiInfo[api].apiName << std::endl;
        std::vector<std::string> devices = v_apiInfo[api].devices;
        for(unsigned dev=0; dev<devices.size(); ++dev){
            std::cout << "\t Device " << dev << " : " << devices[dev] << std::endl;
        }
    }
}

int main(void)
{

    try{
        // if(display drivers) displayDrivers and exit
        // if(ops) api = op1, device = op2
        //
        // verifyApiDevice(op1, op2)
        //playSoundFile("file", op1, op2)

        displayDrivers();
    //    playSoundFile("media/test.wav");
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

