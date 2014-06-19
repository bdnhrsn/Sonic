#ifndef AUDIOOBJ_H
#define AUDIOOBJ_H

#include <stdio.h>

class AudioObj {

  private:
	
	float location [3];
	int index;
	bool active;
	float volume;
	FILE * audioFile;

  public:

	//Creates a new audio object at the world's
	// origin, {0,0,0}.
	AudioObj::AudioObj();

	//Creates a new audio object at the location
	// specified by the parameter.
	AudioObj::AudioObj(float loc[]);
	
	//Returns the array of the object's location.
	float* getLocation();

	//Sets the volume of the audio object. This
	// will only accept values from 0 to 1.
	void setVolume(float vol);

	//Returns the volume of the audio object.
	// This will be a value from 0 to 1.
	float getVolume();

	//Changes the object's location to that which
	// is specifies in the parameter.
	void setLocation(float loc[]);
	void setLocation(float x, float y, float z);

	//Returns whether or not the object is active
	bool isActive();

	//Changes whether or not the object is active
	void setActive(bool active);

	FILE * getAudioFile();

	void setAudioFile(FILE * pFile);
};

#endif