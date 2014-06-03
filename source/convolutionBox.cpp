#include <algorithm>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>

#include "../include/fft.h"
#include "../include/wav.h"

using namespace std;

struct wavFileData
{
	long n;
	int sampleRate;
	int bitDepth;
	int channels;
};


int main()
{
	const double PI = atan(1.0)*4;

	string inFile = "assets\\radioactive.mp3";
	string impFile =  "assets\\impulse1mono.wav";

	complex *input = NULL, *filter = NULL;
	wavFileData inp, fil;

	//LOAD THE WAV FILES
	cout << "Attempting to load wav files..." << endl << endl;
	input = utility::loadCmpWavData(inFile, &inp.n, &inp.sampleRate, &inp.bitDepth, &inp.channels);
	filter = utility::loadCmpWavData(impFile, &fil.n, &fil.sampleRate, &fil.bitDepth, &fil.channels);
	cout << "Wav files loading complete!" << endl;

	cout << "Signal size is " << inp.n << endl;
	cout << "Filter size is " << fil.n << endl;
	cout << "Sampling rate is " << inp.sampleRate << endl;


	//Set up convolution output array.
	complex *output = NULL;
	long NFFT = pow(2, (int)(log(inp.n)/log(2)) + 1);
	NFFT = 2*NFFT;
	cout << "NFFT was: " << NFFT << endl;
	

	//Perform convolution.
	//output = CFFT::stereoConvT(input, filter, filter, inp.n,fil.n, fil.n, NFFT);
	output = CFFT::convolutionT(input, filter, inp.n, fil.n, NFFT);
	cout << "Convolution complete!\n";

	//Scale output from -1 to 1.
	int maxAmp = 1;
	for(long i = 0; i < NFFT; i++)
		if(output[i].re() > maxAmp)
			maxAmp = output[i].re();

	for(long i = 0; i < NFFT; i++)
		output[i] /= maxAmp;
	
	//Output all real numbers of time domain to real.txt 
	CFFT::storingData(output, NFFT,"real.txt",'r');


	delete input;
	delete filter;

	return 0;
}
