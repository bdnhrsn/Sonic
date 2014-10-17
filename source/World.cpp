#include "../include/World.h"
//TODO: function order should match header file

World::~World() {
    //TODO: Figure out how best to delete vector<AudioObj *> World::objList
    // maybe use boost::pointer_vector ("smart pointers"?)
    // http://stackoverflow.com/questions/15942706/c-deleting-vector-of-pointers
    // but for now:
    
    while(!objList.empty()) {
        delete objList.back();
        objList.pop_back();
    }
}

Player& World::getPlayer() {
    return player;
}

Location World::getPlayerLocation() const{
    return this->player.getLocation();
}

AudioObj* World::getAudioObj (size_t index) const{
    return objList.at(index);
}

int World::getNumObj()
{
    return (int)objList.size();
}

AudioObj* World::addAudioObj(const std::string wavFileName){
    objList.push_back(new AudioObj(wavFileName));
	return(getAudioObj(objList.size()-1));
}

AudioObj* World::addAudioObj(const Location& loc, const Velocity& vel, const std::string wavFileName) {
    objList.push_back(new AudioObj(loc,vel, wavFileName));
	return(getAudioObj(objList.size()-1));
}

void World::setPlayerPosition(VariableForLocation x, VariableForLocation y, VariableForLocation z)
{
    player.setLocation(x, y, z);
}

void World::setPlayerBearing(float bearing)
{
    player.setBearing(bearing);
}

void * World::writeAudioObjects (void* obj) {
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

void World::createWriteThread (void) {
    if (!isWriteThreadCreated){
        pthread_create(&writeThread, nullptr, writeAudioObjects, &objList);
        isWriteThreadCreated = true;
    }
}
