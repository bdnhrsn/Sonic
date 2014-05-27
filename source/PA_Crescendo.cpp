#include "portaudio.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

#define CHANNELS 2
#define SAMPLE_RATE 44100
#define BUFFER_SIZE 256

//These are my own variables, don't worry about them.
#define A_F 000
#define NUM_SECONDS 10
#ifndef M_PI
#define M_PI 4*atan(1.0)
#endif

typedef struct
{
	double time; //Global time tracking in seconds.
	char message[30];
	float amplitude;
}
stereoTime;

//This is continuously called asynchrously so long as the P
static int paCallback(const void* inputBuffer, 
					  void* outputBuffer,
					  unsigned long framesPerBuffer,
					  const PaStreamCallbackTimeInfo* timeInfo,
					  PaStreamCallbackFlags statusFlags,
					  void* userData ) //This is a general memory pointer to your "global" variables, which are defined in your structure at the top of the program.
									   //It's essentially to use global variables; however, instead of global definitions, they're packaged in a struct and the memory
									   //address to the struct is given here in the callback function arguments for easy access.
{
	stereoTime* data = (stereoTime*) userData; //This is my structure. It has a time variable I'm going to use in my calcs. Use whatever structure you defined at the top.
	float* out = (float*) outputBuffer; //Cast our output buffer as an array of floating point values.
	unsigned long i;

	//Generate a sine wave in each channel.
	for(i = 0; i < framesPerBuffer; i++)
	{
		data->time += 1/(float)SAMPLE_RATE; //Time in seconds passed in one frame (sample) index is equivalent to 1 / SAMPLE_RATE.

		//Left and right are adjacent memory locations, so two memory increments totals one frame increment. Ex. [12345678] ~ [LRLRLRLR] = 4 frames
		*out++ = (data->amplitude)*(float)sin(2 * M_PI * A_F * data->time);
		*out++ = (data->amplitude)*(float)sin(2 * M_PI * A_F * data->time);
	}

	return paContinue;
}

int main()
{
	PaStreamParameters outputParameters;
    PaStream *stream;
    PaError err;
    stereoTime data;

	data.time = 0;
	data.amplitude = 1;
	sprintf( data.message, "No Message" );
	printf("Generating sine wave of frequency %d %d %d\n", A_F, SAMPLE_RATE, BUFFER_SIZE);

	//BEGIN SETTING UP PORTAUDIO DEVICES
	err = Pa_Initialize();
	if(err != paNoError) goto error;

	outputParameters.device = Pa_GetDefaultOutputDevice();
	if (outputParameters.device == paNoDevice)
	{
		fprintf(stderr, "Error: No default output device.\n");
		goto error;
	}
	outputParameters.channelCount = CHANNELS;
	outputParameters.sampleFormat = paFloat32;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	//Open stream.   ( &stream, in ch #, out ch #, sample rate, frames in buffer, streamflags, callbackname, userdata )
	if( Pa_OpenStream( &stream, NULL, &outputParameters, SAMPLE_RATE, BUFFER_SIZE, NULL, paCallback, &data) != paNoError ) goto error;
    if( Pa_StartStream( stream ) != paNoError ) goto error;
	

    printf("Play for %d seconds.\n", NUM_SECONDS);
    
	clock_t start, current;
	start = clock();
	current = clock();


	while((current - start)/(float)CLOCKS_PER_SEC <= NUM_SECONDS) //We're running for a specific number of seconds here, but think of this condition instead as while the game is running.
	{ 
		//Think of continuously updating our game information here. The data is updated in our userData struct, and while the stream is running asynchronously,
		//it will also be constantly looking in real time at the state of the data in the userData struct and reacting appropriately.

		data.amplitude = (float)(((float)(current - start)/((float)CLOCKS_PER_SEC))/(float)NUM_SECONDS); //This is where the amplitude is changing.
		printf("%f\n", (float)(((float)(current - start)/((float)CLOCKS_PER_SEC))/(float)NUM_SECONDS));

		current = clock();
	}

	//Let's finish up here.
    if( Pa_StopStream( stream ) != paNoError ) goto error;
    if( Pa_CloseStream( stream ) != paNoError ) goto error;

    Pa_Terminate();
    printf("Test finished.\n");
    
    return err;
error:
	printf("%s", Pa_GetErrorText(err));
	return 0;
}