<<<<<<< HEAD
#include <algorithm>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>


#include "../include/Mixer3D.h"	
=======
#include "../include/Mixer3D.h"
>>>>>>> parent of a259e76... Made a bit of progress.
#include "../include/mit_hrtf_lib.h"
#include "../include/wav.h"






using namespace std;

struct wavFileData
{
	long n;
	int sampleRate;
	int bitDepth;
	int channels;
};

Mixer3D::Mixer3D(int bufSize, int smpRate, int bitD, World* w)
{
	myWorld = w;
	bufferSize = bufSize;
	sampleRate = smpRate;
	bitDepth = bitD;

	input = new complex[bufferSize];
	output = new complex*[World::MAX_OBJ];
	outputLeft = new complex*[World::MAX_OBJ];
	outputRight = new complex*[World::MAX_OBJ];
	for (int i = 0; i < World::MAX_OBJ; i++)
	{
		output[i] = new complex[bufferSize * 2];
<<<<<<< HEAD
		outputLeft[i] = new complex[bufferSize];
		outputRight[i] = new complex[bufferSize];
	}
=======
>>>>>>> parent of a259e76... Made a bit of progress.
	result = new complex[bufferSize * 2];
	cbResult = new short[bufferSize * 2];
	lFil = new short[MIT_HRTF_44_TAPS];
	rFil = new short[MIT_HRTF_44_TAPS];
	clFil = new complex[MIT_HRTF_44_TAPS];
	crFil = new complex[MIT_HRTF_44_TAPS];
}

void Mixer3D::mix()
{
<<<<<<< HEAD
	string inFile = "assets\\input1mono.wav";
	wavFileData inp;
	//LOAD THE WAV FILES
	cout << "Attempting to load wav files..." << endl << endl;
	input = utility::loadCmpWavData(inFile, &inp.n, &inp.sampleRate, &inp.bitDepth, &inp.channels);
	cout << "Wav files loading complete!" << endl;

	int Azimuth = 50;
	int elevation = 0;
	HRTFLoading(&Azimuth, &elevation, sampleRate, 1, clFil, crFil);
	convolution(input, clFil, output[0], bufferSize, nTaps, bufferSize);

}

complex *Mixer3D::getLeftFilter()
{
	return clFil;
}
void Mixer3D::convolution(complex *input, complex *filter,complex *output, long NSIG, long NFIL, long &NFFT)
{
	//Check for invalid inputs.
	if (input == NULL || filter == NULL)
	{
		cout << "Could not perform convolution on empty arrays!" << endl;
		return;
	}

	bool NFFTChanged = false;
	//If NFFT not a power of 2, or it is smaller than signal or filter, prompt for new.
	while (log(NFFT) / log(2) != (int)(log(NFFT) / log(2)) || NFFT < NSIG || NFFT < NFIL)
	{
		cout << "Please input a valid NFFT, which is >= NSIG(" << NSIG << ") and >= NFIL(" << NFIL << ") : ";
		cin >> NFFT;
		NFFTChanged = true;
	}

	
	complex *fInput=input, *fFilter=filter;

	
	
	//Perform FFT on both input and filter.
	CFFT::Forward(fInput, NFFT);
	CFFT::Forward(fFilter, NFFT);

	for (int i = 0; i < NFFT; i++)
		output[i] = fInput[i] * fFilter[i];

	CFFT::Inverse(output, NFFT);
	CFFT::Inverse(fInput, NFFT);
	CFFT::Inverse(fFilter, NFFT);
=======
	reset();
	AOList = myWorld->getActiveObjects();
	int nActive = myWorld->getNumActiveObjects();
	for (int i = 0; i < nActive; i++)
		process(AOList[i], i);
>>>>>>> parent of a259e76... Made a bit of progress.
}