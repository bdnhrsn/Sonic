#include "AL/al.h"
#include "AL/alc.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string>
#include <Windows.h>
#include <iostream>
#include <math.h>
#include <time.h>

using namespace std;

bool loadWAVFile(const string fname, ALuint* buffer,
	ALsizei* size, ALsizei* frequency, ALenum* format);

struct RIFF_Header {
	char chunkID[4];
	long chuckSize;
	char format[4];
};

struct WAVE_Format {
	char subChunkID[4];
	long subChunkSize;
	short audioFormat;
	short numChannels;
	long sampleRate;
	long byteRate;
	short blockAlign;
	short bitsPerSample;
};

/*
	Struct that holds data of WAVE file.
*/
struct WAVE_Data {
	char subChunkID[4];
	long subChunk2Size;
};

int main(int argc, char *argv[]) {
	ALCdevice* device = alcOpenDevice(NULL);

	if (!device)
		cerr << "There is no device. There was never a device." << endl;

	ALCenum error;
	alcGetError(device);
	
	error = alcGetError(device);
	if (error != ALC_NO_ERROR)
		cerr << error << endl;

	// Initialize ALContext
	ALCcontext *context;

	context = alcCreateContext(device, NULL);

	if (!alcMakeContextCurrent(context))
		cerr << "Failed to make context current" << endl;

	// Create audio object
	ALuint source[2];

	ALfloat source0Pos[]={3, 0, 6};
	ALfloat source1Pos[]={-2, 0, -1};
	ALfloat listenerPos[]={0, 0, 0};
	 
	// Initialize source parameters
	alGenSources(2, &source[0]);

	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);
	 
	alSourcefv(source[1], AL_POSITION, source1Pos);
	alSourcei(source[1], AL_LOOPING, AL_TRUE);
	
	alListenerfv(AL_POSITION, listenerPos);

	// Create buffer
	ALuint buffer;
	ALsizei size;
	ALsizei freq;
	ALenum fmt;

	if (!loadWAVFile("caliente.wav", &buffer, &size, &freq, &fmt))
		cout << "WAVE file not loaded to memory correctly" << endl;

	alSourcei(source[0], AL_BUFFER, buffer);
	alSourcePlay(source[0]);

	if (!loadWAVFile("water.wav", &buffer, &size, &freq, &fmt))
		cout << "WAVE file not loaded to memory correctly" << endl;

	alSourcei(source[1], AL_BUFFER, buffer);
	alSourcePlay(source[1]);
	
	clock_t start, current;
	start = clock();
	current = clock();

	ALint source_state;

	alGetSourcei(source[0], AL_SOURCE_STATE, &source_state);
	while (source_state == AL_PLAYING) {
		listenerPos[2] = (float)((current - start)/(float)CLOCKS_PER_SEC)/2;
		alListenerfv(AL_POSITION, listenerPos);

		current = clock();
		alGetSourcei(source[0], AL_SOURCE_STATE, &source_state);
	}

	alDeleteSources(1, &source[0]);
	alDeleteBuffers(1, &buffer);
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);

	cin.ignore();
}

/*
	Loads wave file into a buffer
*/
bool loadWAVFile(const string fname, ALuint* buffer,
	ALsizei* size, ALsizei* frequency, ALenum* format) {
	
	// Local var declarations
	FILE* soundFile = NULL;
	WAVE_Format wave_format;
	RIFF_Header riff_header;
	WAVE_Data wave_data;
	unsigned char* data;

	try {
		soundFile = fopen(fname.c_str(), "rb");
		if (!soundFile)
			throw(fname);

		// Read in the first chunk into the struct
		fread(&riff_header, sizeof(RIFF_Header), 1, soundFile);

		// Read in first chunk to RIFF_Header
		if ((riff_header.chunkID[0] != 'R' ||
			riff_header.chunkID[1] != 'I' ||
			riff_header.chunkID[2] != 'F' ||
			riff_header.chunkID[3] != 'F') ||
			(riff_header.format[0] != 'W' ||
			riff_header.format[1] != 'A' ||
			riff_header.format[2] != 'V' ||
			riff_header.format[3] != 'E')) {
				throw ("Invalid RIFF or WAVE Header");}

		// Read in second chunk to WAVE_Format
		fread(&wave_format, sizeof(WAVE_Format), 1, soundFile);////////////////
		// Check for format tag in memory
		if (wave_format.subChunkID[0] != 'f' ||
			wave_format.subChunkID[1] != 'm' ||
			wave_format.subChunkID[2] != 't' ||
			wave_format.subChunkID[3] != ' ')
			throw ("Invalid Wave Format");

		// Check for extra parameters
		if (wave_format.subChunkSize > 16) {
			int extraSize = wave_format.subChunkSize - 16;
			fseek(soundFile, sizeof(char)*extraSize, SEEK_CUR);
		}

		// Read last byte of data before sound data
		fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);
		// Check for data tag in memory
		if (wave_data.subChunkID[0] != 'd' ||
			wave_data.subChunkID[1] != 'a' ||
			wave_data.subChunkID[2] != 't' ||
			wave_data.subChunkID[3] != 'a')
			throw ("Invalid data header");
		
		// Allocate memory for wave data
		data = new unsigned char[wave_data.subChunk2Size];



		// Read in sound data to soundData var
		if (!fread(data, wave_data.subChunk2Size, 1, soundFile))
			throw ("error loading WAVE data into struct");

		// Set variables passed in
		*size = wave_data.subChunk2Size;
		*frequency = wave_format.sampleRate;

		// Set format data - num of channels and bits per sample
		if (wave_format.numChannels == 1) {
			if (wave_format.bitsPerSample == 8)
				*format = AL_FORMAT_MONO8;
			else if (wave_format.bitsPerSample == 16)
				*format = AL_FORMAT_MONO16;
		}
		else if (wave_format.numChannels == 2) {
			if (wave_format.bitsPerSample == 8)
				*format = AL_FORMAT_STEREO8;
			else if (wave_format.bitsPerSample == 16)
				*format = AL_FORMAT_STEREO16;
		}

		// Create OpenAL buffer
		alGenBuffers(1, buffer);
		// Check error
		// Fill OpenAL buffer
		alBufferData(*buffer, *format, (void*)data,
			*size, *frequency);
		// Check error

		// Clean up and return true if successful
		fclose(soundFile);
		return true;
	}
	catch (string error) {
		// Caught a bad file name
		cerr << error << " : trying to load "
			<< fname << endl;
		// Clean up memory if wave loading fails
		if (soundFile != NULL)
			fclose(soundFile);
		return false;
	}
}