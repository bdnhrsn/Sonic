#include<iostream>

using namespace std;

#include "fft.h"
#include "complex.h"
#include "AudioObj.h"
#include "World.h"
#include "mit_hrtf_lib.h"

#ifndef MIXER3D_H
#define MIXER3D_H


//Run the Test.cpp for tesing
//Also, remove the convolutionBox before running the Test.cpp

class Mixer3D
{
public:
	Mixer3D(int bufSize, int smpRate, int bitD, World *w);

	void mix(short *ioDataLeft, short *ioDataRight);//doing everything but now just used for every progress.
	int HRTFLoading(int* pAzimuth, int* pElevation, unsigned int samplerate, unsigned int diffused, complex *&leftFilter, complex *&rightFilter);//loading filters according to the azimuth and elevation values
	
	
	template <typename T>
	void write(std::ofstream& stream, const T& t);
	template <typename SampleType>
	void writeWAVData(const char* outFile, SampleType* buf, size_t bufSize, int sampleRate, short channels);

	void convolution(complex *input, complex *filter, complex *output, long nSig, long nFil, long nFFT);
	void stereoConvolution(complex *input, complex *leftFilter, complex *rightFilter, complex *leftOutput, complex *rightOutput, long nSig, long nFil, long nFFT);
	void overlapConvolution(int Azimuth, int elevation, short *ioDataLeft,short *ioDataRight);
	void stereoTest();

	complex *getLeftFilter();
	long *getTemp()//used to achieve whatever field
	{
		return cbResult;
	}
	void test()
	{
		int AzimuthTemp = 0;
		int Elevation = -10;
		for (int i = 0; i < 64*bufferSize; i++)
		{
			inputTemp[0][i] = input[0][i];
		}

		string s = "orignal.txt";
		ofstream outputFile(s.c_str());

		if (outputFile.is_open())
		{
			for (int i = 0; i < 64 * bufferSize; i++)
			{
				outputFile << inputTemp[0][i].re() << endl;
			}
			outputFile.close();
		}

		complex *filter = new complex[64 * bufferSize];
		nTaps = HRTFLoading(&AzimuthTemp, &Elevation, 44100, 1, clFil, crFil);
		stereoConvolution(inputTemp[0], clFil, crFil, outputLeft[0], outputRight[0], 64 * bufferSize, 1, 64 * bufferSize);
		for (int i = 0; i < 64 * bufferSize; i++)
		{
			cbResult[i] = outputLeft[0][i].re();
		}

	}

private:
	//It is named 
	World *myWorld;
	
	//clFil stands for complex type left filter, left and right means the left and right channel
	complex **input, **outputLeft, **outputRight, *clFil, *crFil;
	
	complex **inputTemp;//This is the variable used to store the data gotten from the world
	complex	*inputTempTemp1;//just define it temporarily to store a small chunk of the input for processing
	
	//overlap stores the data chunk which needs to add up with the next chunk of data
	//to fix the glitch problem
	complex **overlapLeft,**overlapRight;//This is overlap result coming out of the filters
	complex *overlapInput;//This is the input which is used for going through the filter for the overlap

	AudioObj **AOList;//This will not be necessary anymore after the code merging

	short *lFil, *rFil;//left and right filter
	long *cbResult;

	//store the output for small bufferSize
	long *cbResultSmall;
	
	//nTaps represents the size of the filter data, *begin and *end are the arrays 
	//which store the beginning and ending point of the data which is read in every time. 
	long bufferSize, sampleRate, bitDepth,nTaps, dataSize;
	

	long maxTemp=0;

	int nObj;
	void reassignWorld(World* w);
};

#endif