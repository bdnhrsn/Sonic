#include <vector>
#include "../include/AudioObj.h"
#include "../include/World.h"
#include "../include/Player.h"

World::World() {
	player = Player();
	numObj = 0;
	threshold = .05;
}

World::World(float loc[]) {
	player = Player(loc);
	float initLoc[] = {loc[0],loc[1],loc[2]};
	numObj = 0;
	player.setLocation(initLoc);
	threshold = .05;
}

World::World(float loc[], float bear) {
	player = Player(loc);
	float initLoc[] = {loc[0],loc[1],loc[2]};
	numObj = 0;
	player.setLocation(initLoc);
	player.setBearing(bear);
	threshold = .05;
}

World * World::getWorld() {
	return this;
}

Player * World::getPlayer() {
	return &player;
}

float * World::getPlayerLocation() {
	return player.getLocation();
}

int World::addAudioObj() {
	AudioObj obj;
	objList.push_back(new AudioObj());
	numObj++;
	return numObj-1;
}

int World::addAudioObj(float loc[]) {
	AudioObj obj(loc);
	objList.push_back(new AudioObj(loc));
	numObj++;
	return numObj-1;
}

AudioObj * World::getAudioObj(int index) {
	AudioObj *atObj = objList.at(index);
	return atObj;
}

void World::updateActiveObjects() {
	for (int i = 0; i < numObj; i++) {
		if (getPlayer()->getRelativeVolume(getAudioObj(i)) <= threshold)
			getAudioObj(i)->setActive(false);
		else
			getAudioObj(i)->setActive(true);
	}
}

int World::getNumActiveObjects() {
	int n = 0;
	
	for(int i = 0; i < numObj; i++) {
		if (getAudioObj(i)->isActive())
			n++;
	}

	return n;
}

AudioObj ** World::getActiveObjects() {
	AudioObj ** activeObjects = new AudioObj*[MAX_OBJ];
	int j = 0;

	for(int i = 0; i < numObj; i++) {
		if (getAudioObj(i)->isActive()) {
			activeObjects[j] = getAudioObj(i);
			j++;
		}
	}

	return activeObjects;
}