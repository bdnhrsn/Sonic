#include<iostream>

using namespace std;

#include "fft.h"
#include "complex.h"
#include "AudioObj.h"
#include "World.h"
#include "mit_hrtf_lib.h"

#ifndef MIXER3D_H
#define MIXER3D_H

#define INVALID_ANGLE 9999


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
        delete [] input;
        delete [] outputLeft;
        delete [] outputRight;
        delete [] leftFilter;
        delete [] rightFilter;
        delete [] complexLeftFilter;
        delete [] complexRightFilter;
        delete [] overlapLeft;
        delete [] overlapRight;
        delete [] overlapInput;
        delete [] azimuths;
        delete [] elevations;
        delete [] prevAzimuths;
        delete [] prevElevations;
    }

private:
	World *myWorld;
	
	// complexLeftFilter stands for complex type left filter
	complex **outputLeft, **outputRight, **complexLeftFilter, **complexRightFilter;
	
	complex	*input; // just define it temporarily to store a small chunk of the input for processing
	
	// overlap stores the data chunk which needs to add up with the next chunk of data
	// to fix the glitch problem
	complex **overlapLeft, **overlapRight; // This is overlap result coming out of the filters
	complex *overlapInput; // This is the input which is used for going through the filter for the overlap
    complex *fInput, *fFilter;
    
    short * leftFilter, *rightFilter; // left and right filter
	 
	unsigned int bufferSize, sampleRate, bitDepth, filterLength, dataSize;
	
	long maxTemp = 0;
    
    /**
     Returns true if x is a power of two, false otherwise
     */
    bool isPowerOfTwo(int x);
    
    int *prevAzimuths, *azimuths, *elevations, *prevElevations;
};

#endif