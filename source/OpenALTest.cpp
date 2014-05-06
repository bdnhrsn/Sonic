#include "als.h"
#include "alc.h"
#include <math.h>
#include <time.h>
#include <Windows.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	
	
	ALCdevice* device = alcOpenDevice(NULL);

	if (!device)
		cerr << "No device, baby" << endl;

	ALCenum error;
	alcGetError(device);
	
	error = alcGetError(device);
	if (error != ALC_NO_ERROR)
		cerr << error << endl;

	// Start ALContext
	ALCcontext *context;

	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context))
		cerr << "Failed to make context current" << endl;

	// Create audio object
	ALuint source;

	alGenSources((ALuint)1, &source);
	// check error
	alSourcef(source, AL_PITCH, 1);
	// check error
	alSourcef(source, AL_GAIN, 1);
	// check error
	alSource3f(source, AL_POSITION, 0, 0, 0);
	// check error
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	// check error
	alSourcei(source, AL_LOOPING, AL_FALSE);
	// check error

	// Create buffer
	ALuint buffer;
	ALsizei size;
	ALsizei freq;
	ALenum fmt;

	if (!utility::loadWAVFile("shakuhachi.wav", &buffer, &size, &freq, &fmt))
		cout << "WAVE file not loaded to memory correctly" << endl;

	alSourcei(source, AL_BUFFER, buffer);
	// Check error

	alSourcePlay(source);
	// Check error

	ALint source_state;

	alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	// Check error
	while (source_state == AL_PLAYING) {
		alGetSourcei(source, AL_SOURCE_STATE, &source_state);
		// Check error
	}
	

	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}