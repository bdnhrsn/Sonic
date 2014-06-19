#ifndef WORLD_H
#define WORLD_H

#include "stdafx.h"
#include <vector>
#include "AudioObj.h"
#include "Room.h"
#include "Listener.h"

class World {

  private:
	Listener listener;
	std::vector<AudioObj *> objList;
	int numObj;
	std::vector<Room *> roomList;
	int numRoom;
	float threshold;
	
  public:
	static const int MAX_OBJ = 20;

	//This default contrustor creates a listener at
	// the world's origin, {0,0,0}.
	World::World();

	//This constructor creates a listener at the
	// location specified by the parameter.
	World::World(float loc[]);

	//This constructor creates a listener at the
	// location specified by the first parameter,
	// and sets the listener's bearing specified by
	// the second parameter.
	World::World(float loc[], float bear);

	//Returns a reference to the listener.
	Listener * getListener();

	//Returns the array of the listener's location.
	float* getListenerLocation();

	//Adds an audio object to the world. Returns the
	// index of the created object. Sets the location
	// of the created object at the world's origin, {0,0,0}.
	int addAudioObj();

	//Adds an audio object to the world. Returns the
	// index of the created object. Sets the location
	// of the created object at the location specified 
	// by the parameter.
	int addAudioObj(float loc[]);

	//Returns a reference to the audio object at the
	// specified index.
	AudioObj * getAudioObj(int index);

	//Adds an room to the world. Returns the
	// index of the created object. Sets the number
	// of corners of the room to the first parameter. Uses
	// the arrays xList and zList to respectively set the 
	// x and z coordinates of each corner. 
	// Optionally, specify the room height. This defaults at 3.0. 
	// Optionally, specify the floor position. This defaults at 0.
	int addRoom(int n, float xList[], float zList[]);
	int addRoom(int n, float xList[], float zList[], float roomHeight);
	int addRoom(int n, float xList[], float zList[], float roomHeight, float floorPos);

	//Returns a reference to the room at the
	// specified index.
	Room * getRoom(int index);

	//Updates each of the audio object's "active" value, based
	// on the object's relative volume.
	void updateActiveObjects();

	//Returns the number of objects that are active.
	int getNumActiveObjects();

	//Returns an array of pointers to the world's active objects.
	AudioObj ** getActiveObjects();

};

#endif