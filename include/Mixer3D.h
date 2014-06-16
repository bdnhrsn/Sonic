#include "complex.h"
#include "AudioObj.h"
#include "World.h"
#include "fft.h"

#ifndef MIXER3D_H
#define MIXER3D_H

<<<<<<< HEAD

//Run the Test.cpp for tesing
//Also, remove the convolutionBox before running the Test.cpp
=======
>>>>>>> parent of a259e76... Made a bit of progress.
class Mixer3D
{
public:
	Mixer3D(int bufSize, int smpRate, int bitD, World* w);
<<<<<<< HEAD
	void mix();//doing everything but now just used for every progress.
	int HRTFLoading(int* pAzimuth, int* pElevation, unsigned int samplerate, unsigned int diffused, complex *&leftFilter, complex *&rightFilter);//loading filters according to the azimuth and elevation values
	template <typename T>
	void write(std::ofstream& stream, const T& t);//writing data but mostly no need to care about this funciton
	template <typename SampleType>
	void writeWAVData(const char* outFile, SampleType* buf, size_t bufSize, int sampleRate, short channels);//used to write txt. into wav.
	void Mixer3D::convolution(complex *input, complex *filter, complex *output, long NSIG, long NFIL, long &NFFT);

	complex *getLeftFilter();//retrieving leftFilter for debugging
private:
	
	//There is still one pointer which is being created and deleted everytime when the filter fetch function is called.
	//It is named const short* psTempTaps in the mit_hrtf_lib.c file
=======
	void mix();
private:
>>>>>>> parent of a259e76... Made a bit of progress.
	World *myWorld;
	complex *input, **output,**outputLeft,**outputRight, *result, *clFil, *crFil;
	AudioObj **AOList;
	short *lFil, *rFil, *cbResult;
<<<<<<< HEAD
	long bufferSize, sampleRate, bitDepth, nTaps;//nTaps represents the size of the filter data
=======
	int bufferSize, sampleRate, bitDepth;
>>>>>>> parent of a259e76... Made a bit of progress.
	void reassignWorld(World* w);
};

#endif