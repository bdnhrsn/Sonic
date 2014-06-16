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

	void mix();//doing everything but now just used for every progress.
	int HRTFLoading(int* pAzimuth, int* pElevation, unsigned int samplerate, unsigned int diffused, complex *&leftFilter, complex *&rightFilter);//loading filters according to the azimuth and elevation values
	
	
	template <typename T>
	void write(std::ofstream& stream, const T& t);
	template <typename SampleType>
	void writeWAVData(const char* outFile, SampleType* buf, size_t bufSize, int sampleRate, short channels);

	void convolution(complex *input, complex *filter, complex *output, long NSIG, long NFIL, long &NFFT);
	void stereoConvolution(complex *input, complex *leftFilter, complex *rightFilter, complex *leftOutput, complex *rightOutput, long NSIG, long NFIL, long &NFFT);

	complex *getLeftFilter();
	short *getTemp()
	{
		return cbResult;
	}

private:
	//It is named 
	World *myWorld;
	complex **input, **output, **outputLeft,**outputRight,*result, *clFil, *crFil;
	AudioObj **AOList;
	short *lFil, *rFil, *cbResult;

	long bufferSize, sampleRate, bitDepth,nTaps;//nTaps represents the size of the filter data
	int nObj;
	void reassignWorld(World* w);
};

#endif