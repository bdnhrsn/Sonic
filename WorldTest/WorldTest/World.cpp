#include "stdafx.h"
#include <vector>
#include "AudioObj.h"
#include "World.h"
#include "Listener.h"

World::World() {
	listener = Listener();
	numObj = 0;
	numRoom = 0;
	threshold = .05;
}

World::World(float loc[]) {
	listener = Listener(loc);
	float initLoc[] = {loc[0],loc[1],loc[2]};
	numObj = 0;
	numRoom = 0;
	listener.setLocation(initLoc);
	threshold = .05;
}

World::World(float loc[], float bear) {
	listener = Listener(loc);
	float initLoc[] = {loc[0],loc[1],loc[2]};
	numObj = 0;
	numRoom = 0;
	listener.setLocation(initLoc);
	listener.setBearing(bear);
	threshold = .05;
}

Listener * World::getListener() {
	return &listener;
}

float * World::getListenerLocation() {
	return listener.getLocation();
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

Room * World::getRoom(int index) {
	Room *atObj = roomList.at(index);
	return atObj;
}

int World::addRoom(int n, float xList[], float zList[]) {
	Room room(n, xList, zList);
	roomList.push_back(new Room(room));
	numRoom++;
	return numRoom-1;
}

int World::addRoom(int n, float xList[], float zList[], float roomHeight) {
	Room room(n, xList, zList, roomHeight);
	roomList.push_back(new Room(room));
	numRoom++;
	return numRoom-1;
}

int World::addRoom(int n, float xList[], float zList[], float roomHeight, float floorPos) {
	Room room(n, xList, zList, roomHeight, floorPos);
	roomList.push_back(new Room(room));
	numRoom++;
	return numRoom-1;
}

AudioObj * World::getAudioObj(int index) {
	AudioObj *atObj = objList.at(index);
	return atObj;
}

void World::updateActiveObjects() {
	for (int i = 0; i < numObj; i++) {
		if (getListener()->getRelativeVolume(getAudioObj(i)) <= threshold)
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