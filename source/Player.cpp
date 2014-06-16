#include "stdafx.h"
#include "Math.h"
#include "Player.h"
#include <iostream>

Player::Player() {
	location[0] = 0;
	location[1] = 0;
	location[2] = 0;
	bearing = 90;
}

Player::Player(float loc[]) {
	location[0] = loc[0];
	location[1] = loc[1];
	location[2] = loc[2];
	bearing = 90;
}

Player::Player(float loc[], float bear) {
	location[0] = loc[0];
	location[1] = loc[1];
	location[2] = loc[2];
	bearing = bear;
}

float* Player::getLocation() {
	return location;
}

void Player::setLocation(float loc[]) {
	location[0] = loc[0];
	location[1] = loc[1];
	location[2] = loc[2];
}

void Player::setLocation(float x, float y, float z) {
	location[0] = x;
	location[1] = y;
	location[2] = z;
}

float Player::getBearing() {
	return bearing;
}

void Player::setBearing(float bear) {
	bearing = bear;
}

// This works!
float Player::getRadius(AudioObj * obj) {
	return sqrt(pow(obj->getLocation()[0] - location[0],2) 
		+ pow(obj->getLocation()[1] - location[1],2) 
		+ pow(obj->getLocation()[2] - location[2],2));
}

// DOES NOT WORK WITH HEAD TRACKING
float Player::getZenith(AudioObj * obj) {
	if (getRadius(obj) == 0)
		return 0;
	else
		return (float)(asin((obj->getLocation()[1] - location[1]) / getRadius(obj)) * R2D);
}

// DOES NOT WORK WITH HEAD TRACKING
float Player::getAzimuth(AudioObj * obj) {
	float dx = obj->getLocation()[0] - location[0];
	float dz = obj->getLocation()[2] - location[2];
	float angle;
	if (dx == 0) {
		if (dz > 0)
			angle = 90;
		else if (dz < 0)
			angle = 270;
		else
			angle = 0;
	} else if (dz == 0) { 
		if (dx < 0)
			angle = 180;
		else
			angle = 0;
	} else {
		angle = (float)(atan(dz/dx) * R2D);
		if (dx < 0)
			angle = 180 + angle;
		else if (dz < 0 && dx > 0)
			angle = 360 + angle;
	}
	return fmod(angle - bearing + 450,360);
}

// DOES NOT WORK WITH HEAD TRACKING
float Player::getLeftAzimuth(AudioObj * obj) {
	return fmod(getAzimuth(obj) + 270, 360);
}

// DOES NOT WORK WITH HEAD TRACKING
float Player::getRightAzimuth(AudioObj * obj) {
	return fmod(getAzimuth(obj) + 90, 360);
}

// DOES NOT WORK WITH HEAD TRACKING
float * Player::getOrientation(AudioObj * obj) {
	float orientation[3];
	orientation[0] = getRadius(obj);
	orientation[1] = getZenith(obj);
	orientation[2] = getAzimuth(obj);
	return orientation;
}

float Player::getRelativeVolume(AudioObj * obj) {
	float volume = obj->getVolume() * (1.0 / pow(getRadius(obj), 2));
	if (volume < obj->getVolume())
		return volume;
	else
		return obj->getVolume();
}