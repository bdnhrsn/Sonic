#include "../include/AudioObj.h"
//#include <iostream>

Location AudioObj::getLocation () const {
    return this->location;
}

void AudioObj::setLocation (const Location& loc) {
    this->location = loc;
}

void AudioObj::setLocation (VariableForLocation x, VariableForLocation y, VariableForLocation z) {
    this->location = Location(x,y,z);
}

Velocity AudioObj::getVelocity () const {
    return this->velocity;
}

void AudioObj::setVelocity (const Velocity& vel) {
    this->velocity = vel;
}

void AudioObj::setVelocity (VariableForVelocity dx, VariableForVelocity dy, VariableForVelocity dz) {
    this->velocity = Velocity(dx,dy,dz);
}

float AudioObj::getVolume() const {
    return this->volume;
}

void AudioObj::setVolume(float vol) {
    if (vol > 1 || vol < 0){
        throw invalid_argument ("volume out of range(0-1)");
    }
    this->volume = vol;
}

bool AudioObj::isActive() const {
    return this->active;
}

void AudioObj::setActive(bool active){
    this->active = active;
}

void AudioObj::setRandomVolume(){
    float rndNum = rand()%100+1;
    this->volume = rndNum/100;
    std::cout << "Random volume : " << volume << endl;
    wavObject.volume = rndNum/100;
}



/*
void AudioObj::loadWavFile() {
	const string inFile = "input1mono.wav";
	tempBufferWavFile = utility::loadCmpWavData(inFile, &(wavFileData.n), &(wavFileData.sampleRate), &(wavFileData.bitDepth), &(wavFileData.channels));
    currentTrackerPosition =0;
}*/

/*
void AudioObj::fillAudioData (complex* target, unsigned int length) {
    if (currentTrackerPosition + length < wavFileData.n){
        for (unsigned int i=0; i<length; i++){
            target[i] = tempBufferWavFile[i+currentTrackerPosition];
        }
        currentTrackerPosition += length;
    }else if (currentTrackerPosition < wavFileData.n) {
        for (unsigned int i=0; i< wavFileData.n-currentTrackerPosition; i++){
            target[i] = tempBufferWavFile[i+currentTrackerPosition];
        }
        for (unsigned int i=0; i< length - (wavFileData.n-currentTrackerPosition); i++){
            target[i] = 0;
        }
        currentTrackerPosition += length;
    }else {
        if (repeat) {
            currentTrackerPosition =0;
            fillAudioData(target, length);
            return;
        }else {
            for (unsigned int i=0; i<length; i++){
                target[i] = 0;
            }
        }
    }
}*/

void AudioObj::fillAudioData (complex* target, unsigned int length) {
    //cout<<"read size remaining, length"<<circBuff.readSizeRemaining()<<","<<length<<endl;
    //std::cout<< "In fillAudioData"<<endl;
    circBuff.read(target, length);
}

void AudioObj::writeCircBuff() {
    
    unsigned int length = circBuff.writeSizeRemaining();
    if(length>16384) {
        //cout<<"In write circ Buff : "<<length<<endl;
        wavObject.loadMoreData(length, repeat);
        circBuff.write(wavObject.complexTempData, length);
    }
    
    //circBuff.write(tempBufferWavFile+currentTrackerPosition+8192, length);
    //currentTrackerPosition += length;
}