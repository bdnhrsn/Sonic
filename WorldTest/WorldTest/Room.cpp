#include "stdafx.h"
#include "Room.h"

Room::Room(int n, float xList[], float zList[]) {
	for (int i = 0; i < n; i++) {
		xVertex.push_back(xList[i]);
		zVertex.push_back(zList[i]);
	}
	nVertices = n;
	height = 3;
	floorPosition = 0;
}

Room::Room(int n, float xList[], float zList[], float roomHeight) {
	for (int i = 0; i < n; i++) {
		xVertex.push_back(xList[i]);
		zVertex.push_back(zList[i]);
	}
	nVertices = n;
	height = roomHeight;
	floorPosition = 0;
}

Room::Room(int n, float xList[], float zList[], float roomHeight, float floorPos) {
	for (int i = 0; i < n; i++) {
		xVertex.push_back(xList[i]);
		zVertex.push_back(zList[i]);
	}
	nVertices = n;
	height = roomHeight;
	floorPosition = floorPos;
}

void Room::setFloorPosition(float y) {
	floorPosition = y;
}

float Room::getFloorPosition() {
	return floorPosition;
}

void Room::setHeight(float newHeight) {
	height = newHeight;
}

float Room::getHeight() {
	return height;
}

float * Room::getVertex(int i) {
	float v[2] = {xVertex.at(i), zVertex.at(i)};
	return v;
}

bool Room::contains(Listener * listener) {
	if (listener->getLocation()[1] < floorPosition || listener->getLocation()[1] > (floorPosition + height)) {
		return false;
	} else {
		int i;
		int j = nVertices - 1;
		float x = listener->getLocation()[0];
		float z = listener->getLocation()[2];
		bool isOdd = false;
			
		for	(i=0; i<nVertices; i++) {
			if (zVertex.at(i) < z && zVertex.at(j) >= z ||  zVertex.at(j) < z && zVertex.at(i) >= z) {
				if (xVertex.at(i) + (z - zVertex.at(i)) / (zVertex.at(j) - zVertex.at(i)) * (xVertex.at(j) - xVertex.at(i)) < x) {
					isOdd =! isOdd;
				}
			}
			j = i;
		}
		return isOdd;
	}
}

bool Room::contains(AudioObj * obj) {
	if (obj->getLocation()[1] < floorPosition || obj->getLocation()[1] > (floorPosition + height)) {
		return false;
	} else {
		int i;
		int j = nVertices;
		float x = obj->getLocation()[0];
		float z = obj->getLocation()[2];
		bool isOdd = false;
			
		for	(i=0; i<nVertices; i++) {
			if (zVertex.at(i) < z && zVertex.at(j) >= z ||  zVertex.at(j) < z && zVertex.at(i) >= z) {
				if (xVertex.at(i) + (z - zVertex.at(i)) / (zVertex.at(j) - zVertex.at(i)) * (xVertex.at(j) - xVertex.at(i)) < x) {
					isOdd =! isOdd;
				}
			}
			j = i;
		}
		return isOdd;
	}
}