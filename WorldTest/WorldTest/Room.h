#ifndef ROOM_H
#define ROOM_H

#include <vector>
#include "AudioObj.h"
#include "Listener.h"

class Room {

  private:
	std::vector<float> xVertex;
	std::vector<float> zVertex;
	int nVertices;
	float floorPosition;
	float height;
	
  public:
	//Sets the number
	// of corners of the room to the first parameter. Uses
	// the arrays xList and zList to respectively set the 
	// x and z coordinates of each corner. 
	// Optionally, specify the room height. This defaults at 3.0. 
	// Optionally, specify the floor position. This defaults at 0.
	Room(int n, float xList[], float zList[]);
	Room(int n, float xList[], float zList[], float roomHeight);
	Room(int n, float xList[], float zList[], float roomHeight, float floorPos);
	
	//Returns true only if the listener is within the room.
	bool contains(Listener * listener);
	
	//Returns true only if the audio object is within the room.
	bool contains(AudioObj * obj);

	//Sets the floor position of the room to the parameter's value.
	void setFloorPosition(float y);

	//Returns the value of the floor position.
	float getFloorPosition();

	//Sets the height of the room to the parameter's value.
	void setHeight(float newHeight);

	//Returns the height of the room.
	float getHeight();

	//Returns a two member float array containing the x and z
	// coordinates of the vertex at the specified index.
	float * getVertex(int i);

};

#endif