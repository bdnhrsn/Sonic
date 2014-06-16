#include <algorithm>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>


#include "../include/Mixer3D.h"	
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
		outputLeft[i] = new complex[bufferSize];
		outputRight[i] = new complex[bufferSize];
	}
	result = new complex[bufferSize * 2];
	cbResult = new short[bufferSize * 2];
	
	switch (smpRate)
	{
	case 44100:
		nTaps = MIT_HRTF_44_TAPS;
	case 48000:
		nTaps = MIT_HRTF_48_TAPS;
	case 88200:
		nTaps = MIT_HRTF_88_TAPS;
	case 96000:
		nTaps = MIT_HRTF_96_TAPS;
	}
	lFil = new short[nTaps];
	rFil = new short[nTaps];
	clFil = new complex[nTaps];
	crFil = new complex[nTaps];

}

int Mixer3D::HRTFLoading(int* pAzimuth, int* pElevation, unsigned int samplerate, unsigned int diffused, complex *&leftFilter, complex *&rightFilter)
{

	int size = mit_hrtf_get(pAzimuth, pElevation, samplerate, diffused, &lFil, &rFil);


	for (int i = 0; i < size; i++)
	{
		leftFilter[i] = (lFil[i]) / 1.0;
		rightFilter[i] = (rFil[i]) / 1.0;
	}

	return size;
}

template <typename T>
void Mixer3D::write(std::ofstream& stream, const T& t)
{
	stream.write((const char*)&t, sizeof(T));
}

template <typename SampleType>
void Mixer3D::writeWAVData(const char* outFile, SampleType* buf, size_t bufSize, int sampleRate, short channels)
{
	std::ofstream stream(outFile, std::ios::binary);                // Open file stream at "outFile" location

	/* Header */
	stream.write("RIFF", 4);                                        // sGroupID (RIFF = Resource Interchange File Format)
	write<int>(stream, 36 + bufSize);                               // dwFileLength
	stream.write("WAVE", 4);                                        // sRiffType

	/* Format Chunk */
	stream.write("fmt ", 4);                                        // sGroupID (fmt = format)
	write<int>(stream, 16);                                         // Chunk size (of Format Chunk)
	write<short>(stream, 1);                                        // Format (1 = PCM)
	write<short>(stream, channels);                                 // Channels
	write<int>(stream, sampleRate);                                 // Sample Rate
	write<int>(stream, sampleRate * channels * sizeof(SampleType)); // Byterate
	write<short>(stream, channels * sizeof(SampleType));            // Frame size aka Block align
	write<short>(stream, 8 * sizeof(SampleType));                   // Bits per sample

	/* Data Chunk */
	stream.write("data", 4);                                        // sGroupID (data)
	stream.write((const char*)&bufSize, 4);                         // Chunk size (of Data, and thus of bufferSize)
	stream.write((const char*)buf, bufSize);                        // The samples DATA!!!
}

void Mixer3D::mix()
{
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
}