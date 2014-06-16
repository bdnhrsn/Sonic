#include<iostream>
#include "../include/AudioObj.h"

AudioObj::AudioObj() {
	location[0] = 0;
	location[1] = 0;
	location[2] = 0;
	active = false;
	volume = 1;
}

AudioObj::AudioObj(float loc[]) {
	location[0] = loc[0];
	location[1] = loc[1];
	location[2] = loc[2];
	active = false;
	volume = 1;
}

float* AudioObj::getLocation() {
	return location;
}

void AudioObj::setLocation(float loc[]) {
	location[0] = loc[0];
	location[1] = loc[1];
	location[2] = loc[2];
}

float AudioObj::getVolume() {
	return volume;
}

void AudioObj::setVolume(float vol) {
	if (vol <= 1 && vol >= 0)
		volume = vol;
	else
		printf("Invalid volume");
}

void AudioObj::setLocation(float x, float y, float z) {
	location[0] = x;
	location[1] = y;
	location[2] = z;
}

bool AudioObj::isActive() {
	return active;
}

void AudioObj::setActive(bool activeBool) {
	if (activeBool)
		active = true;
	else
		active = false;
}