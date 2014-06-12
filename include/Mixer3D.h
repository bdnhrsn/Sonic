#include "complex.h"
#include "AudioObj.h"
#include "World.h"

#ifndef MIXER3D_H
#define MIXER3D_H



class Mixer3D
{
public:
	Mixer3D(int bufSize, int smpRate, int bitD, World* w);
	void mix();
	int HRTFLoading(int* pAzimuth, int* pElevation, unsigned int samplerate, unsigned int diffused, complex *&leftFilter, complex *&rightFilter);
	template <typename T>
	void write(std::ofstream& stream, const T& t);
	template <typename SampleType>
	void writeWAVData(const char* outFile, SampleType* buf, size_t bufSize, int sampleRate, short channels);

	complex *getLeftFilter();
private:
	//There is still one pointer which is being created and deleted everytime when the filter fetch function is called.
	//It is named 
	World *myWorld;
	complex *input, **output, *result, *clFil, *crFil;
	AudioObj **AOList;
	short *lFil, *rFil, *cbResult;
	int bufferSize, sampleRate, bitDepth, nTaps;//nTaps represents the size of the filter data
	void reassignWorld(World* w);

};

#endif