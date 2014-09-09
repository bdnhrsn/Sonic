#include<iostream>

using namespace std;

#include "fft.h"
#include "complex.h"
#include "AudioObj.h"
#include "World.h"
#include "mit_hrtf_lib.h"

#ifndef MIXER3D_H
#define MIXER3D_H


class Mixer3D
{
public:
	Mixer3D(int bufSize, int smpRate, int bitD, World *w);

    // Re-computes and stores elevation and azimuth angles for all
    // audioObjects in this->myWorld
    void updateAngles();
    
	int HRTFLoading(int* pAzimuth, int* pElevation, unsigned int samplerate, unsigned int diffused, complex *&leftFilter, complex *&rightFilter);//loading filters according to the azimuth and elevation values
	
	void convolution(complex *input, complex *filter, complex *output, long nSig, long nFil, long nFFT);
	void stereoConvolution(complex *input, complex *leftFilter, complex *rightFilter, complex *leftOutput, complex *rightOutput, long nSig, long nFil, long nFFT);
	void overlapConvolution(short *ioDataLeft,short *ioDataRight);
    
    
    ~Mixer3D()
    {
        delete [] inputTempTemp1;
        delete [] outputLeft;
        delete [] outputLeft;
        delete [] lFil;
        delete [] rFil;
        delete [] clFil;
        delete [] crFil;
        delete [] overlapLeft;
        delete [] overlapRight;
        delete [] overlapInput;
        delete [] prevAzimuths;
        delete [] Azimuths;
        delete [] elevations;
        delete [] prevElevations;
    }

private:
	World *myWorld;
	
	// clFil stands for complex type left filter
	complex **outputLeft, **outputRight, **clFil, **crFil;
	
	complex	*inputTempTemp1; // just define it temporarily to store a small chunk of the input for processing
	
	// overlap stores the data chunk which needs to add up with the next chunk of data
	// to fix the glitch problem
	complex **overlapLeft, **overlapRight; // This is overlap result coming out of the filters
	complex *overlapInput; // This is the input which is used for going through the filter for the overlap
    complex *fInput, *fFilter;
    
    short *lFil, *rFil; // left and right filter
	 
	unsigned int bufferSize, sampleRate, bitDepth, nTaps, dataSize;
	
	long maxTemp = 0;
    
    int *prevAzimuths, *Azimuths, *elevations, *prevElevations;
};

#endif