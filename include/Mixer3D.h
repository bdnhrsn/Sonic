#include "fft.h"
#include "complex.h"
#include "AudioObj.h"
#include "World.h"
#include "mit_hrtf_lib.h"

#ifndef MIXER3D_H
#define MIXER3D_H

/**
Certain features of this 3D Mixer class are optimized for Sonic and thus 
include many hard-coded pre-allocations. For this reason, it is not 
recommended to use these methods for purposes other than to Sonic's
3D mixing, as they will likely break or give undefined results.

If you wish to make features more general purpose or create your own, it is 
recommended that you define and pre-allocate your own containers as data 
members of the 3D Mixer or make sure they comply with the size restrictions
of the 3D mixing code.
*/

#define INVALID_ANGLE 9999

class Mixer3D
{
public:
	Mixer3D(int bufSize, int smpRate, int bitD, World *w);
    ~Mixer3D();

    /**
    Pre: The 3DMixer object must be instantiated and have reference to a list of Sonic AudioObj objects to perform processing.
	Post: Simply call the method and the 3D Mixer will populate the ioDataLeft and ioDataRight arrays with 16-bit signed integer formatted audio data. This
	  	  audio data is the processed result of 3D mixing on the current buffers of data in the audio objects and their current 3D positions. The ioData 
		  arrays must be equal to the bufferSize of the 3D Mixer.
	*/
	void performMix(short *ioDataLeft, short *ioDataRight);

private:
	/**
	Pre: Azimuth must be an angle between -180 and 180. Elevation must be between -90 and 90. The sample rate must be either 44100, 48000, 88200, or 96000 
	 	 KHz. leftFilter and rightFilter must be pre-allocated according to the number of taps for each sample rate: 128, 140, 256, or 279.
	Post: leftFilter and rightFilter will be filled with complex data in the frequency domain which represent the HRTF filter at a specified Azimuth 
		  and Elevation.
	*/
	int loadHRTF(int* pAzimuth, int* pElevation, unsigned int samplerate, unsigned int diffused, complex *&leftFilter, complex *&rightFilter);
	
	/**
	Pre: The input and filter can be any size relative to each other, as this  method will perform self-contained zero-padding. As such, NFFT must be a 
		 power of 2 equal to or greater than the larger of the two. All three containers must be pre-allocated and the output length must be equal to 
		 NFFT. Because of the nature of the 3D Mixer, the NFFT maximum size is 2*bufferSize. Additionally, two containers for the frequency domain 
		 versions of the input and filter (size 2*bufferSize) are used and already pre-allocated by the 3D Mixer class. nSig and nFil are the numerical sizes
		 of the input and filter arrays.
	Post: Output will be filled with the result in the time domain of the input and filter being circularly convolved through a frequency domain dot product. 
		  The output size will be NFFT samples. There is a side effect of fInput and fFilter being populated with the frequency domain data which was used for 
		  the convolution.
	*/
	void convolution(complex *input, complex *filter, complex *output, long nSig, long nFil, long nFFT);
	
	/**
	Pre: Similar requirements to the normal convolution(), extended to left and right versions of filter and output. As usual, all containers must be 
		 pre-allocated and nFFT must be at maximum 2*bufferSize.
	Post: The method will call convolution() twice for both left and right audio data, using the leftFilter and rightFilter and storing the results in
		  leftOutput and rightOutputThis method will propagate the fInput/fFilter population side effect of convolution().
	*/
	void stereoConvolution(complex *input, complex *leftFilter, complex *rightFilter, complex *leftOutput, complex *rightOutput, long nSig, long nFil, long nFFT);
	
    /**
    Re-computes and stores elevation and azimuth angles for all
    audioObjects in this->myWorld
    */
    void updateAngles();

    /**
     Returns true if x is a power of two, false otherwise
     */
    bool isPowerOfTwo(int x);
    
    //Data members

	unsigned int bufferSize, 				//The size of the audio frames.
				 sampleRate, 				//The sample rate of the audio.
				 bitDepth, 					//The bit depth of the audio.
				 filterLength; 					//The MIT KEMAR Filter Size.
	
	World *myWorld; 						//A pointer to the world.
    Player &player;                        //A reference to myWorld's player
    
    short *leftFilter, *rightFilter; 		//Arrays for retrieving the integer formatted filter data from the MIT KEMAR HRTF Database.
    int *prevAzimuths, *prevElevations,
    	*azimuths, *elevations;
    
	complex	*inputAO, 					 	//Holds the current input of each audio object.
			*overlapInput,					//Holds the input of the last iteration in case the filter changed and the tail needs recalculation.
    		*fInput, *fFilter,				//Data arrays to hold frequency domain representation of an input and filter. Used in convolution().
			**complexLeftFilter, **complexRightFilter,			 	//Holds the complex datatype versions of the current filter.
			**outputLeft, **outputRight, 	//Holds the output of each current input with the current filter.
			**overlapLeft,**overlapRight;   //Holds the second half of each 2*bufferSize convolution for next iteration.
};

#endif
