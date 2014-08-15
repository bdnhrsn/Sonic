#include "../include/World.h"

Player* World::getPlayer() {
    return &(this->player);
}

Location World::getPlayerLocation() const{
    return this->player.getLocation();
}

AudioObj* World::addAudioObj(const std::string wavFileName){
    objList.push_back(new AudioObj(wavFileName));
	return(getAudioObj(objList.size()-1));
	//return numObj-1;
}

AudioObj* World::addAudioObj(const Location& loc, const Velocity& vel, const std::string wavFileName) {
    objList.push_back(new AudioObj(loc,vel, wavFileName));
	return(getAudioObj(objList.size()-1));
	//return numObj-1;
}


AudioObj* World::getAudioObj (size_t index) const{
    return objList.at(index);
}

void * World::writeAudioObjects (void* obj) {
    cout<<"in thread write audio objects" <<endl;
    vector<AudioObj *> *objListCurrent = static_cast<vector<AudioObj *>*>(obj);
    while (1){
        size_t size = objListCurrent->size();
        for (int i=0; i < size ; i++){
            (*objListCurrent)[i]->writeCircBuff();
        }
        usleep(10000);
    }
    return NULL;
}
