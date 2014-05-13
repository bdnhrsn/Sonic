#include "al.h"
#include "alc.h"
#include <stdio.h>
#include <string>
#include <math.h>
#include <ctime>

#ifndef M_PI
#define M_PI atan(1.0)*4
#endif

#define CHANNELS 2
#define SAMPLE_RATE 48000
#define BITS 16
#define T_FRAMES 2400
#define NUM_BUFFERS 2

using namespace std;

//This will automatically assume 16-bit audio. It will not work otherwise, will fix this later.
void fillSine(ALvoid *bufferData, short channels, unsigned long frames, int sampleRate, float frequency, short amplitude)
{
	double time = 0;
	short *data = (short*)bufferData, val;

	for(unsigned long i = 0; i < frames; i++)
	{
		time += 1 /(float)sampleRate;
		val = (short)(amplitude * sin(2 * M_PI * frequency * time));

		for(short j = 0; j < channels; j++)
		{
			*data++ = val;
		}
	}
}

void fillSaw(ALvoid *bufferData, short channels, unsigned long frames, int sampleRate, float frequency, short amplitude)
{
	double time = 0;
	short *data = (short*)bufferData;
		double val;

	for (unsigned long i = 0; i < frames; i++)
	{
		time += 1 / (double)sampleRate;
		if (time >= 1 / frequency)
			time -= 1 / frequency;

		val = (2 * amplitude* frequency * time - amplitude);
		//printf("the value is %lf. \n", val);

		for (short j = 0; j < channels; j++)
		{
			*data++ = (short)val;
		}
	}
}

int main()
{
	ALCenum error;
	ALCdevice *device;
	device = alcOpenDevice(NULL);
	if(!device)
		printf("Device not found!\n");

	ALboolean enumeration;
	enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	if (enumeration == AL_FALSE)
		printf("no enumeration supported!\n");

	ALCcontext *context;
	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context))
		printf("Context creation failed!");

	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	alListener3f(AL_POSITION, 0, 0, 1.0f);
	alListener3f(AL_VELOCITY, 0, 0, 0);
	alListenerfv(AL_ORIENTATION, listenerOri);

	ALuint source;
	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_LOOPING, AL_FALSE);



	ALuint *buffer = (ALuint*)malloc(sizeof(ALuint)*NUM_BUFFERS);
	alGenBuffers(NUM_BUFFERS, buffer);

	ALsizei dataSize = T_FRAMES * CHANNELS * (BITS/8);
	ALvoid *bufferData = (ALvoid *)malloc(dataSize);
	float mPitch = 440;
	fillSaw(bufferData, CHANNELS, T_FRAMES, SAMPLE_RATE, mPitch, 25000);

	for(int i = 0; i < NUM_BUFFERS; i++)
		alBufferData(buffer[i], AL_FORMAT_STEREO16, bufferData, dataSize, SAMPLE_RATE);	
	alSourceQueueBuffers(source, NUM_BUFFERS, buffer);
	alSourcePlay(source);

	clock_t start, current;
	start = clock();
	current = clock();

	short buffID = 0, totalBuffers = 0;
	while(true)
	{
		ALint processed;
		alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
		while(processed--)
		{
			alSourceUnqueueBuffers(source, 1, &buffer[buffID]);
			fillSaw(bufferData, CHANNELS, T_FRAMES, SAMPLE_RATE, mPitch, 25000);
			printf("Refilled the sine data into buffer: %d, Pitch is %f\n", buffID, mPitch);
			alBufferData(buffer[buffID], AL_FORMAT_STEREO16, bufferData, dataSize, SAMPLE_RATE);
			alSourceQueueBuffers(source, 1, &buffer[buffID]);

			buffID++; totalBuffers++;
			//mPitch += totalBuffers;
			if(buffID == NUM_BUFFERS)
				buffID = 0;
		}
	}

	
	
	
	
	
	
	// cleanup context
	alDeleteSources(1, &source);
	alDeleteBuffers(NUM_BUFFERS, buffer);
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);

	return 0;
}