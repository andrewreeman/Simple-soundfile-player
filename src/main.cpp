
#include <sndfile.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <exception>
#include <tclap/CmdLine.h>
#include <fstream>

#include "../include/portaudio.h"
#include "../include/AudioIO.hh"
#include "../include/Factory_AudioIO.hh"
#include "../include/ioUtils.hh"
#include "../include/SoundFile.hh"

#define PI 3.14159265

#define DEFAULT_OUTPUT_DEVICE -1
#define DEFAULT_API -1

const char* PROGRAM_NAME = "Simple Soundfile Player";

void displayDrivers();

bool fileExists(std::string filePath);
void argParse(int argc, char** argv, std::string* filePath, bool* isDisplayDriversOn, AudioIOType* api, int* dev);
AudioInOut::AudioIOType intToAudioIOType(int api);
int devCheckValidity(int api, int dev);

int main(int argc, char** argv)
{
    std::string filePath;
    bool isDisplayDriversOn;
    AudioIOType audioInOutApi;
    int deviceNumber;
    try{
        argParse(argc, argv, &filePath, &isDisplayDriversOn, &audioInOutApi, &deviceNumber);
        if(isDisplayDriversOn){
            displayDrivers();
            return 0;
        }
        if( fileExists(filePath) ){
            playSoundFile(filePath.c_str(), audioInOutApi, deviceNumber);
            return 0;
        }
        else{
            std::cerr << "Error: " << filePath << " is not a valid file path." << std::endl;
            return 1;
        }
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
	std::vector<DevInfo> apiDevices;

    for(unsigned api=0; api<v_apiInfo.size(); ++api){
        std::cout << "Api " << api << " : " << v_apiInfo[api].apiName << std::endl;
		apiDevices = v_apiInfo[api].devices;
        for(unsigned dev=0; dev<apiDevices.size(); ++dev){
            std::cout << "\t Device " << dev << " : " << apiDevices[dev].devName << std::endl;
            std::cout << "\t \t Max inputs: " << apiDevices[dev].numInputs << std::endl;
            std::cout << "\t \t Max outputs: " << apiDevices[dev].numOutputs << std::endl;
        }
    }
}

bool fileExists(std::string filePath){
	std::ifstream file(filePath.c_str(), std::ios::binary);
	if(!file) return false;
	return true;
}

void argParse(int argc, char** argv, std::string* filePath, bool* isDisplayDriverOn, AudioIOType* api, int* deviceNumber){
	TCLAP::CmdLine cmd("Test for argument parsing", ' ', "1.0");
	TCLAP::UnlabeledValueArg<std::string> filePathArg("name", "The path to the audio file.", true, "", "string");
	TCLAP::SwitchArg dispDrivArg("D", "DisplayDriver", "Displays drivers and exits.", false);
	TCLAP::ValueArg<int> apiArg("a", "api", "The api to use.", false, DEFAULT_API, "integer");
	TCLAP::ValueArg<int> devArg("d", "dev", "The device to use.", false, DEFAULT_OUTPUT_DEVICE, "integer");

	cmd.xorAdd(filePathArg, dispDrivArg);
	cmd.add(apiArg);
	cmd.add(devArg);
	cmd.parse(argc, argv);

	*filePath = filePathArg.getValue();
	*isDisplayDriverOn = dispDrivArg.getValue();
	try{
	  *api = ::intToAudioIOType( apiArg.getValue() );
	  *deviceNumber = devCheckValidity( apiArg.getValue(), devArg.getValue() );
	}
	catch(std::exception& error){
			std::cerr << "Api argument error: " << error.what() << " \n Using default api instead." << std::endl;
			*api = AudioIOType::PA_DEFAULT;
	}
}

AudioInOut::AudioIOType intToAudioIOType(int api){

    if(DEFAULT_API == api) return AudioInOut::AudioIOType::PA_DEFAULT;
    std::vector<ApiInfo> v_ApiInf;
    std::vector<DevInfo> v_ApiDevices;
    if(api < 0) throw std::runtime_error("Api value must be positive or zero.");
    v_ApiInf = AudioInOut::getHostApis();
    if( api > (signed int)v_ApiInf.size() ) throw std::runtime_error("Api value does not match an available api.");
    v_ApiDevices = v_ApiInf[api].devices;
    if( (signed int)v_ApiDevices.size() == 0 ) throw std::runtime_error("Api has no devices.");
    return AudioInOut::intToAudioIOType(api);
}

int devCheckValidity(int api, int dev){
    // Checks if the device number is valid
    if(dev == DEFAULT_OUTPUT_DEVICE) return DEFAULT_OUTPUT_DEVICE;
    if(dev < 0){
        std::cerr << "Device number must be positive, zero or -1 (default device). Using default device." << std::endl;
        return DEFAULT_OUTPUT_DEVICE;
    }
    std::vector<ApiInfo> v_ApiInf;
    std::vector<DevInfo> v_ApiDevices;
    v_ApiInf = AudioInOut::getHostApis();
    v_ApiDevices = v_ApiInf[api].devices;
    if( dev > (signed int)v_ApiDevices.size() ){
        std::cerr << "Device number is above the number of devices. Using default device." << std::endl;
        return DEFAULT_OUTPUT_DEVICE;
    }
    return dev;
}
