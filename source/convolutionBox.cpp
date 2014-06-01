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

	string inFile = "assets\\input1mono.wav";
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

	//Set up convolution output array.
	complex *output = NULL;
	long NFFT = pow(2, (int)(log(inp.n)/log(2)) + 1);
	cout << "NFFT was: " << NFFT << endl;

	//Perform convolution.
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
	ofstream outputFile("real.txt");
	if(outputFile.is_open())
	{
		for(int i = 0; i < NFFT; i++)
			outputFile << output[i].re() << endl;
		outputFile.close();
	}

	delete input;
	delete filter;

	return 0;
}
