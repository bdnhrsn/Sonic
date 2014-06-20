#include <algorithm>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>

#include "../include/Mixer3D.h"
#include "../include/World.h"
#include "../include/fft.h"
#include "../include/wav.h"
#include "../include/mit_hrtf_lib.h"
using namespace std;


struct wavFileData
{
	long n;
	int sampleRate;
	int bitDepth;
	int channels;
};

Mixer3D::Mixer3D(int bufSize, int smpRate, int bitD, World *w)
{
	myWorld = w;
	bufferSize = bufSize;
	sampleRate = smpRate;
	bitDepth = bitD;
	nObj = w->getNumActiveObjects();
	
	
	//this is just used for testing
	nObj = 1;
	//this is just used for testing

	input = new complex*[World::MAX_OBJ];
	inputTemp = new complex*[World::MAX_OBJ];
	outputLeft = new complex*[World::MAX_OBJ];
	outputRight = new complex*[World::MAX_OBJ];
	inputTempTemp = new complex[bufferSize];
	for (int i = 0; i < World::MAX_OBJ; i++)
	{
		
		input[i] = new complex[bufferSize];
		inputTemp[i] = new complex[bufferSize];
		outputLeft[i] = new complex[bufferSize];
		outputRight[i] = new complex[bufferSize];
	}

	/////This part is just for testing
	string inFile = "assets\\input1mono.wav";
	wavFileData inp;
	//LOAD THE WAV FILES
	cout << "Attempting to load wav files..." << endl << endl;
	input[0] = utility::loadCmpWavData(inFile, &inp.n, &inp.sampleRate, &inp.bitDepth, &inp.channels);
	cout << "Wav files loading complete!" << endl;
	//////This part is just for testing

	begin = new long[World::MAX_OBJ];
	begin[0] = 0;
	end = new long[World::MAX_OBJ];
	testOutput = new short[18 * bufferSize];
	result = new complex[bufferSize];
	cbResult = new short[2*bufferSize];
	cbResultLeft = new short[bufferSize];
	cbResultRight = new short[bufferSize];
	lFil = new short[bufferSize];
	rFil = new short[bufferSize];
	clFil = new complex[bufferSize];
	crFil = new complex[bufferSize];
}

int Mixer3D::HRTFLoading(int* pAzimuth, int* pElevation, unsigned int samplerate, unsigned int diffused, complex *&leftFilter, complex *&rightFilter)
{

	int size = mit_hrtf_get(pAzimuth, pElevation, samplerate, diffused, lFil, rFil);


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


complex *Mixer3D::getLeftFilter()
{
	return clFil;

}
void Mixer3D::convolution(complex *input, complex *filter,complex *output, long nSig, long nFil, long nFFT)
{
	//Check for invalid inputs.
	if (input == NULL || filter == NULL)
	{
		cout << "Could not perform convolution on empty arrays!" << endl;
		return;
	}

	bool NFFTChanged = false;
	//If NFFT not a power of 2, or it is smaller than signal or filter, prompt for new.
	
	while (log2(nFFT) / log2(2) != (int)(log2(nFFT) / log2(2)) || nFFT < nSig || nFFT < nFil)
	{
		cout << "Please input a valid NFFT, which is >= nSig(" << nSig << ") and >= NFIL(" << nFil << ") : ";
		cin >> nFFT;
		NFFTChanged = true;
	}

	
	complex *fInput=input, *fFilter=filter;

	
	
	//Perform FFT on both input and filter.
	CFFT::Forward(fInput, nFFT);
	CFFT::Forward(fFilter, nFFT);

	for (int i = 0; i < nFFT; i++)
		output[i] = fInput[i] * fFilter[i];

	CFFT::Inverse(output, nFFT);
	CFFT::Inverse(fInput, nFFT);
	CFFT::Inverse(fFilter, nFFT);


}
void Mixer3D::stereoConvolution(complex *input, complex *leftFilter, complex *rightFilter, complex *leftOutput, complex *rightOutput, long NSIG, long NFIL, long NFFT)
{
	convolution(input, leftFilter, leftOutput, NSIG, NFIL, NFFT);
	convolution(input, rightFilter, rightOutput, NSIG, NFIL, NFFT);

}
void Mixer3D::mix(short *ioDataLeft,short *ioDataRight)
{
	
		for (int i = 0; i < bufferSize; i++)
		{
			inputTemp[0][i] = input[0][begin[0] + i];
		}

		int Azimuth = 0;
		int elevation = 0;
		nTaps = HRTFLoading(&Azimuth, &elevation, sampleRate, 1, clFil, crFil);
		stereoConvolution(inputTemp[0], clFil, crFil, outputLeft[0], outputRight[0], bufferSize, nTaps, bufferSize);
		
		for (int i = 0; i < bufferSize; i++)
		{
			cbResult[2 * i] = outputLeft[0][i].re();
			cbResult[2 * i + 1] = outputRight[0][i].re();
			
			inputTemp[0][i] = outputLeft[0][i].re();
			


			ioDataLeft[i] = (short)outputLeft[0][i].re();
			ioDataRight[i] = (short)outputRight[0][i].re();
			cbResultLeft[i] = (short)outputLeft[0][i].re();
			cbResultRight[i] = (short)outputRight[0][i].re();

		}
		//ioDataLeft = cbResultLeft;
		//ioDataRight = cbResultRight;
		begin[0] += bufferSize;
}
void Mixer3D::stereoTest()
{
	int step = bufferSize / 128;
	for (int i = 0; i < bufferSize; i++)
	{
		inputTemp[0][i] = input[0][i % 65336];
	}
	int elevation = 0;
	bool flag;
	for (int Azimuth = -180; Azimuth < 180; Azimuth += 5)
	{
		if (Azimuth < 0)
			flag = 1;
		else
			flag = 0;
		int n = ((Azimuth+180) / 5)*(step);
		for (int i = n; i < n + step; i++)
		{
			inputTempTemp[i % (step)] = inputTemp[0][i];
		}

		nTaps = HRTFLoading(&Azimuth, &elevation, sampleRate, 1, clFil, crFil);
		
		if (flag)
			Azimuth = -Azimuth;
		
		stereoConvolution(inputTempTemp, clFil, crFil, outputLeft[0], outputRight[0], step, nTaps, step);

		for (int i = n; i < n+step; i++)
		{
			cbResult[2 * i] = outputLeft[0][i%step].re();
			cbResult[2 * i + 1] = outputRight[0][i%step].re();
		}
		cout << endl;
	}
}

	


void Mixer3D::mixDown()
{
	for (int i = 0; i < nObj; i++)
	{
		for (int j = 0; j < bufferSize; j++)
		{
			cbResultLeft[j] += 1 / nObj*outputLeft[i][j].re();
			cbResultRight[j] += 1 / nObj*outputRight[i][j].re();

		}
	}
}

template void Mixer3D::write< short >(std::ofstream& stream, const short& t);
template void Mixer3D::write< int >(std::ofstream& stream, const int& t);
template void Mixer3D::writeWAVData<short>(const char* outFile, short* buf, size_t bufSize, int sampleRate, short channels);