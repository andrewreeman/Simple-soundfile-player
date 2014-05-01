#include "../include/SoundFile.hh"

void isOutputDevice(DevInfo devInf){
    if(devInf.numOutputs == 0) throw isNotOutputDevice();
}

void SoundFilePlayer::playSoundFile(const char* soundfile, AudioIOType audioApi, int deviceNumber){
    SF_INFO inFile_Inf;
    SNDFILE* inFile = nullptr;
    AudioIO* outputDevice;
    Factory_AudioIO factoryAudioIO;
    m_doStop = false;
    int numChans;
    int blockSize = 0;
    int interleavedBufferSize = 0;
    std::vector<float> inputBuffer_Interleaved;
    DevInfo devInf;
    try{
        inFile = sf_open(soundfile, SFM_READ, &inFile_Inf);
        if(inFile == nullptr) throw sndFileStd_Exception();
        numChans = inFile_Inf.channels;
        blockSize = 0;
        interleavedBufferSize = 0;

        if( (numChans == 1 ) && (audioApi == AudioIOType::PA_JACK) ){
            numChans = 2;
            blockSize = FRAMES_PER_BUFFER * numChans;
            interleavedBufferSize = inFile_Inf.frames * numChans;
            inputBuffer_Interleaved.resize(inFile_Inf.frames + blockSize + blockSize); // mono
            sf_readf_float(inFile, inputBuffer_Interleaved.data(), inFile_Inf.frames);
            std::vector<float> stereoInterleaved = monoToStereoInterleaved(inputBuffer_Interleaved);
            inputBuffer_Interleaved.resize( stereoInterleaved.size() + blockSize + blockSize);
            inputBuffer_Interleaved = stereoInterleaved;
        }
        else{
            blockSize = FRAMES_PER_BUFFER * numChans;
            interleavedBufferSize = inFile_Inf.frames * numChans;
            inputBuffer_Interleaved.resize(interleavedBufferSize + blockSize); // add one block so we don't go over
            sf_readf_float(inFile, inputBuffer_Interleaved.data(), interleavedBufferSize);
        }
        outputDevice = factoryAudioIO.createAudioIO(audioApi, numChans, inFile_Inf.samplerate, FRAMES_PER_BUFFER, deviceNumber);
        devInf = outputDevice->getDevInfo();
        outputDevice->start();
        for(int i=0; i<interleavedBufferSize; i+= blockSize ){
            if(!m_doStop)
                outputDevice->write(&inputBuffer_Interleaved[i]);
            else
                m_doStop = false;
                break;
        }
        outputDevice->stop();
        sf_close(inFile);
    }
    catch(isNotOutputDevice e){
        std::cerr << "Error: " << e.what() << std::endl;
    }
}


void playSoundFile(const char* soundfile, AudioIOType audioApi, int deviceNumber){
    SF_INFO inFile_Inf;
    SNDFILE* inFile = nullptr;
    AudioIO* outputDevice;
    Factory_AudioIO factoryAudioIO;
    inFile = sf_open(soundfile, SFM_READ, &inFile_Inf);
    int numChans = inFile_Inf.channels;
    int blockSize = 0;
    int interleavedBufferSize = 0;
    std::vector<float> inputBuffer_Interleaved;
    DevInfo devInf;
    try{
		inFile = sf_open(soundfile, SFM_READ, &inFile_Inf);
		if(inFile == nullptr) throw sndFileStd_Exception();
		numChans = inFile_Inf.channels;
		blockSize = 0;
		interleavedBufferSize = 0;

        if( (numChans == 1 ) && (audioApi == AudioIOType::PA_JACK) ){
            numChans = 2;
            blockSize = FRAMES_PER_BUFFER * numChans;
            interleavedBufferSize = inFile_Inf.frames * numChans;
            inputBuffer_Interleaved.resize(inFile_Inf.frames + blockSize + blockSize); // mono
            sf_readf_float(inFile, inputBuffer_Interleaved.data(), inFile_Inf.frames);
            std::vector<float> stereoInterleaved = monoToStereoInterleaved(inputBuffer_Interleaved);
            inputBuffer_Interleaved.resize( stereoInterleaved.size() + blockSize + blockSize);
            inputBuffer_Interleaved = stereoInterleaved;
        }
        else{
            blockSize = FRAMES_PER_BUFFER * numChans;
            interleavedBufferSize = inFile_Inf.frames * numChans;
            inputBuffer_Interleaved.resize(interleavedBufferSize + blockSize); // add one block so we don't go over
            sf_readf_float(inFile, inputBuffer_Interleaved.data(), interleavedBufferSize);
        }
        outputDevice = factoryAudioIO.createAudioIO(audioApi, numChans, inFile_Inf.samplerate, FRAMES_PER_BUFFER, deviceNumber);
        devInf = outputDevice->getDevInfo();
        outputDevice->start();
        for(int i=0; i<interleavedBufferSize; i+= blockSize )
            outputDevice->write(&inputBuffer_Interleaved[i]);
        outputDevice->stop();
        sf_close(inFile);
    }
    catch(isNotOutputDevice e){
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void playSine(){
    // For testing.
    int numChans = 2;
    int sampleRate = 44100;
    int duration = 3;
    int numSamps = duration * sampleRate;
    int blockSize = FRAMES_PER_BUFFER * numChans;
    int interleavedBufferSize = numSamps * numChans;
    std::vector<float> inputBuffer_Interleaved(interleavedBufferSize);
    AudioIO* outputDevice;
    Factory_AudioIO factoryAudioIO;
    outputDevice= factoryAudioIO.createAudioIO(AudioIOType::PA_DEFAULT, numChans, sampleRate, FRAMES_PER_BUFFER, -1);

    for(int buffInd=0, i=0; buffInd<numSamps; ++i, buffInd+=numChans){
        for(int chan=0; chan<numChans; ++chan){
            inputBuffer_Interleaved[buffInd+chan] = sin(440.f * TWO_PI * (float)i / sampleRate ) * 0.8;
        }
    }
    outputDevice->start();
    for(int i=0; i<interleavedBufferSize; i+= blockSize )
        outputDevice->write(&inputBuffer_Interleaved[i]);
    outputDevice->stop();
}

