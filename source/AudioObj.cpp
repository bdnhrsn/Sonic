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
        throw invalid_argument("Volume not in range (0-1)");
    }
    this->volume = vol;
}

void AudioObj::setRandomVolume() {
    float randVol = (rand() % 100 + 1) / 100.0;
	this->setVolume(randVol);
}
	
bool AudioObj::isActive() const {
    return this->active;
}

void AudioObj::setActive(bool active){
    this->active = active;
}

void AudioObj::setRepeat(bool rep) {
    this->repeat = rep;
}


bool AudioObj::fillAudioData (complex* target, unsigned int length) {
    
    if(circBuff.readSizeRemaining() < length) {
                return false;
    }
    circBuff.read(target, length);
    return true;
}

void AudioObj::writeCircBuff() {
    if(!toLoadMoreData){
        //return;
    }
    unsigned int length = (unsigned int)circBuff.writeSizeRemaining();
    if(length>16384) {
        //cout<<"In write circ Buff : "<<length<<endl;
        if(!(wavObject.loadMoreData(length, repeat))) {
            toLoadMoreData = false;
            return;
        }
        
        circBuff.write(wavObject.complexTempData, length);
    }
    
}
