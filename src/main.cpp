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
#include <exception>
#include <tclap/CmdLine.h>
#include <fstream>

#include "../include/portaudio.h"
#include "../include/AudioIO.hh"
#include "../include/Factory_AudioIO.hh"
#include "../include/ioUtils.hh"
#include "../include/SoundFile.hh"

#define PI 3.14159265

const char* PROGRAM_NAME = "Simple Soundfile Player";

void displayDrivers();
AudioIOType selectApi(int apiType);

bool fileExists(std::string filePath);
void argParse(int argc, char** argv, std::string* filePath, bool* isDisplayDriversOn);

int main(int argc, char** argv)
{

    try{
        // if(display drivers) displayDrivers and exit
        // if(ops) api = op1, device = op2
        //
        // verifyApiDevice(op1, op2)
        //playSoundFile("file", op1, op2)


        std::string filePath;
        bool isDisplayDriversOn;
        argParse(argc, argv, &filePath, &isDisplayDriversOn);
        if(isDisplayDriversOn){
            displayDrivers();
            return 0;
        }
        if( fileExists(filePath) ){
            playSoundFile(filePath.c_str(), "portaudio_default");
            return 0;
        }
        else{
            std::cerr << "Error: " << filePath << " is not a valid file path." << std::endl;
            return 1;
        }

        //playSine();
        return 0;
    }
    catch(Pa_Exception& paEx){
        std::cerr << paEx.what() << std::endl;
        std::cerr << "PA error number:" << Pa_GetLastHostErrorInfo()->errorCode << std::endl;
        std::cerr << "PA error text:" << Pa_GetLastHostErrorInfo()->errorText << std::endl;
        return 1;
    }
    catch(sndFile_Exception& sndEx){ // catch by reference is important as this will catch the exact child instead of copying and casting to parent
        std::cerr << sndEx.what() << std::endl;
        return 1;
    }
    catch(TCLAP::ArgException &e){
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        return 1;
     }
    catch(std::exception& e ){
        std::cerr << e.what() << std::endl;
        return 1;
    }
    catch(...){
        std::cerr << "Exception occurred" <<std::endl;
        return 1;
    }
}

void displayDrivers(){
    std::vector<ApiInfo> v_apiInfo = AudioInOut::getHostApis();
    for(unsigned api=0; api<v_apiInfo.size(); ++api){
        std::cout << "Api " << api << " : " << v_apiInfo[api].apiName << std::endl;
        std::vector<std::string> devices = v_apiInfo[api].devices;
        std::vector<DevInfo> devs = v_apiInfo[api].devicess;
        for(unsigned dev=0; dev<devices.size(); ++dev){
            std::cout << "\t Device " << dev << " : " << devices[dev] << std::endl;
            std::cout << "\t \t Max inputs: " << devs[dev].numInputs << std::endl;
            std::cout << "\t \t Max outputs: " << devs[dev].numOutputs << std::endl;

        }
    }
}

AudioIOType selectApi(int apiType){
    return AudioIOType::PA_DEFAULT;
}

bool fileExists(std::string filePath){
    std::ifstream file(filePath.c_str(), std::ios::binary);
    if(!file) return false;
    return true;
}


void argParse(int argc, char** argv, std::string* filePath, bool* isDisplayDriverOn){
        TCLAP::CmdLine cmd("Test for argument parsing", ' ', "1.0");
        TCLAP::UnlabeledValueArg<std::string> filePathArg("name", "The path to the audio file.", true, "", "string");
        TCLAP::SwitchArg dispDrivArg("D", "DisplayDriver", "Displays drivers and exits.", false);

        cmd.xorAdd(filePathArg, dispDrivArg);
        cmd.parse(argc, argv);
        *filePath = filePathArg.getValue();
        *isDisplayDriverOn = dispDrivArg.getValue();
}
