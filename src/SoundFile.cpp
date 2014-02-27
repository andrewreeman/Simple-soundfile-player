// This will later be extended into a soundfile class that is passed a pointer to the AudioIO object.

#include "../include/SoundFile.hh"


void playSoundFile(const char* soundfile){
    /* Uses sndfile to open a sound file and read into a buffer.
    Then uses portaudio to play this file.
    */
    SF_INFO inFile_Inf;
    SNDFILE* inFile;
    AudioIO* outputDevice;
    Factory_AudioIO factoryAudioIO;
    inFile = sf_open(soundfile, SFM_READ, &inFile_Inf);
    int blockSize = FRAMES_PER_BUFFER * inFile_Inf.channels;
    int interleavedBufferSize = inFile_Inf.frames * inFile_Inf.channels;
    std::vector<SAMPLE> inputBuffer_Interleaved(interleavedBufferSize + blockSize); // add one block so we don't go over

    outputDevice = factoryAudioIO.createAudioIO("portaudio_alsa", inFile_Inf.channels, inFile_Inf.samplerate, FRAMES_PER_BUFFER, 0);

    sf_readf_float(inFile, inputBuffer_Interleaved.data(), interleavedBufferSize);
    outputDevice->start();

    for(int i=0; i<interleavedBufferSize; i+= blockSize )
        outputDevice->write(&inputBuffer_Interleaved[i]);

    outputDevice->stop();
    sf_close(inFile);
}

void playSine(){
    // For testing.
    int numChans = 1;
    int sampleRate = 44100;
    int duration = 3;
    int numSamps = duration * sampleRate;
    int blockSize = FRAMES_PER_BUFFER * numChans;
    int interleavedBufferSize = numSamps * numChans;
    std::vector<SAMPLE> inputBuffer_Interleaved(interleavedBufferSize);
    AudioIO* outputDevice;
    Factory_AudioIO factoryAudioIO;
    outputDevice= factoryAudioIO.createAudioIO("portaudio_alsa", numChans, sampleRate, FRAMES_PER_BUFFER, 0);


    for(int i=0; i<interleavedBufferSize; ++i){
        inputBuffer_Interleaved[i] = sin(440.f * TWO_PI * (float)i / sampleRate );
    }

    outputDevice->start();
    for(int i=0; i<interleavedBufferSize; i+= blockSize )
        outputDevice->write(&inputBuffer_Interleaved[i]);

    outputDevice->stop();

}

