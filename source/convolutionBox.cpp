#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string>

#include "../include/fft.h"
#include "../include/wav.h"

using namespace std;

int main()
{
	const double PI = atan(1.0)*4;

	string inFile = "C:\\stuff\\input1mono.wav";
	string impFile = "C:\\stuff\\impulse1mono.wav";

	long NSIG, NFIL;
	complex *monoIn = NULL, *monoFil = NULL;
	int inpSmpRate, inpBitD, inpChan, impSmpRate, impBitD, impChan;

	//LOAD THE WAV FILES
	cout << "Attempting to load wav files..." << endl;
	monoIn = utility::loadCmpWavData(inFile, &NSIG, &inpSmpRate, &inpBitD, &inpChan);
	monoFil = utility::loadCmpWavData(impFile, &NFIL, &impSmpRate, &impBitD, &impChan);
	cout << "Wav files loading complete!" << endl;

	cout << "NSIG is " << NSIG << endl;
	cout << "NFIL is " << NFIL << endl;

	//Set up convolution output array.
	complex *output = NULL;
	long NFFT = pow(2, (int)(log(NSIG)/log(2)) + 1);
	cout << "NFFT was: " << NFFT << endl;

	//Perform convolution.
	output = CFFT::convolutionT(monoIn, monoFil, NSIG, NFIL, NFFT);
	cout << "Convolution complete!\n";

	int maxAmp = 1;
	for(long i = 0; i < NFFT; i++)
		if(output[i].re() > maxAmp)
			maxAmp = output[i].re();
	cout << "Max Amp was: " << maxAmp << endl; 

	for(long i = 0; i < NFFT; i++)
		output[i] /= maxAmp;

	delete monoIn;
	delete monoFil;

	return 0;
}

void ffttest()
{
	int NSIG, NFFT, NMAX;
	const int samplingRate = 256;
	const double PI = atan(1.0)*4;

	cout << "The input size is (in samples): ";
	cin >> NSIG ;
	cout << "The FFT size is (in samples): ";
	cin >> NFFT;
	NMAX = max(NSIG, NFFT);

	//   Allocate memory for signal data
	complex *input = new complex[NMAX];
	complex *output = new complex[NMAX];

	for(int i = 0; i < NSIG; i++)
		input[i] = sin(2 * PI * 20 * ((float)i)/samplingRate);
	for(int i = 0; i < NMAX; i++)
		printf("Sample %d is value: %f\n", i, input[i]);
	cout << endl << endl;

	CFFT::Forward(input, output, NFFT);
	for(int i = 0; i < NMAX; i++)
		printf("Output %d is value: %f\n", i, sqrt(output[i].norm()));
}
