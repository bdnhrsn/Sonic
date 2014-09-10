#include <algorithm>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>

#include "../include/Mixer3D.h"
#include "../include/World.h"
#include "../include/fft.h"
#include "../include/mit_hrtf_lib.h"

using namespace std;


struct wavFileData {
	long n;
	int sampleRate;
	int bitDepth;
	int channels;
};

Mixer3D::Mixer3D(int bufSize, int smpRate, int bitD, World *w):myWorld(w), bufferSize(bufSize), sampleRate(smpRate), bitDepth(bitD) {
	outputLeft = new complex*[World::MAX_OBJ];
	outputRight = new complex*[World::MAX_OBJ];
	overlapLeft = new complex*[World::MAX_OBJ];
	overlapRight = new complex*[World::MAX_OBJ];
	
	//This is the variable used to store the data gotten from the world
	input = new complex[2*bufferSize];

	for (int i = 0; i < World::MAX_OBJ; i++) {
		overlapLeft[i] = new complex[bufferSize];
		overlapRight[i] = new complex[bufferSize];
		outputLeft[i] = new complex[2*bufferSize];
		outputRight[i] = new complex[2*bufferSize];
	}

	// for testing
	overlapInput = new complex[2 * bufferSize];

	// These are for filter fetch
	leftFilter = new short[2 * bufferSize];
	rightFilter = new short[2 * bufferSize];

	// These are for storing filters for processing
    complexLeftFilter = new complex*[World::MAX_OBJ];
    complexRightFilter = new complex*[World::MAX_OBJ];
    
    fInput = new complex[2 * bufferSize];
    fFilter = new complex[2 * bufferSize];
    
    azimuths = new int[World::MAX_OBJ];
    elevations = new int[World::MAX_OBJ];
    prevAzimuths = new int[World::MAX_OBJ];
    prevElevations = new int[World::MAX_OBJ];
    
    updateAngles(); // initalize azimuths and elevations
    
    for(int i = 0; i < World::MAX_OBJ; i++) {
        // TODO: Should we initialize the filter values?
        complexLeftFilter[i] = new complex[2*bufferSize];
        complexRightFilter[i] = new complex[2*bufferSize];

        prevAzimuths[i] = INVALID_ANGLE;
        prevElevations[i] = INVALID_ANGLE;
    }
}

void Mixer3D::updateAngles() {
    Player *p1 = myWorld->getPlayer();
    for (int i = 0; i < myWorld->getNumObj(); i++) {
        AudioObj* iAudioObj = myWorld->getAudioObj(i);
        azimuths[i] = p1->computeAzimuth(iAudioObj);
        elevations[i] = p1->computeElevation(iAudioObj);
        printf("azimuths[%d]: %d\n", i, azimuths[i]);
        printf("elevations[%d]: %d\n", i, elevations[i]);
    }
}

bool Mixer3D::isPowerOfTwo(int x) {
    return !(x == 0) && !(x & (x - 1));
}

int Mixer3D::HRTFLoading(int* pAzimuth, int* pElevation, unsigned int samplerate, unsigned int diffused, complex *&leftFilterIn, complex *&rightFilterIn) {
    int size = mit_hrtf_get(pAzimuth, pElevation, samplerate, diffused, leftFilter, rightFilter);

    // TODO: Since cl(r)Fil is passed as left(right)Filter, How do cl(r)Fil and l(r)Fil differ?
    // Also, if leftFilter and rightFilter are complex**, why are we adding doubles to them?
	for (int i = 0; i < size; i++) {
		leftFilterIn[i] = (double)(leftFilter[i]);
		rightFilterIn[i] = (double)(rightFilter[i]);
	}

	return size;
}


void Mixer3D::convolution(complex *input, complex *filter, complex *output, long nSig, long nFil, long nFFT) {

	if (input == NULL || filter == NULL) {
		throw invalid_argument("Input and Filter must be non-NULL.");
	}

	if (!isPowerOfTwo(nFFT) || nFFT < nSig || nFFT < nFil) {
        throw invalid_argument("NFFT must be a power of two, bigger than the signal length, and bigger than the filter length.");
	}

	// Perform FFT on both input and filter.
    // TODO: "Streamline" CFFT class?
	CFFT::Forward(input, fInput, (unsigned int)nFFT);
	CFFT::Forward(filter, fFilter, (unsigned int)nFFT);

	for (int i = 0; i < nFFT; i++) {
		output[i] = fInput[i] * fFilter[i];
    }
	CFFT::Inverse(output, (unsigned int)nFFT);
}

void Mixer3D::stereoConvolution(complex *input, complex *leftFilter, complex *rightFilter, complex *leftOutput, complex *rightOutput, long nSIG, long nFIL, long nFFT) {
	convolution(input, leftFilter, leftOutput, nSIG, nFIL, nFFT);
	convolution(input, rightFilter, rightOutput, nSIG, nFIL, nFFT);
}

void Mixer3D::overlapConvolution( short *ioDataLeft,short *ioDataRight) {
    // Clear the left and right channels
    for(int i = 0; i < bufferSize; i++) {
        ioDataLeft[i] = 0;
        ioDataRight[i] = 0;
    }
    
    // Loop through the audioObjects and do the mixing one sequentially
    
    // TODO: Possible bug? Cuts short if one audioObj is inactive?
    for (int i = 0; i < myWorld->getNumObj() && myWorld->getAudioObj(i)->isActive() == true; i++) {
        // dynamically calculate the Azimuth and Elevation between every object and the player
        updateAngles();
        AudioObj* iAudioObj = myWorld->getAudioObj(i);
        
        // loading in input data for the iteration accordingly
       if (!(iAudioObj->fillAudioData(input, bufferSize))) {
            continue;
       }
        
        // scale volume according to distance of object from player
        // TODO: Is this realistic?
        float iVolume = iAudioObj->getVolume();
        float iDistance = myWorld->getPlayer()->computeDistanceFrom(iAudioObj) ;
        float iAmplitudeFactor = iVolume / iDistance;
        for(int j = 0; j < bufferSize ; j++) {
            input[j] *= iAmplitudeFactor;
        }
        
        // zero pad
        for (int j = bufferSize; j < 2 * bufferSize; j++) {
            input[j] = 0;
        }

        if (azimuths[i] != prevAzimuths[i] ||
           elevations[i] != prevElevations[i]) {
            // object location relative to player has changed, so fetch a new filter
            filterLength = HRTFLoading(&azimuths[i], &elevations[i], sampleRate, 1, complexLeftFilter[i], complexRightFilter[i]);
            
            // zero pad
            for (int j = filterLength; j < 2 * bufferSize; j++) {
                complexLeftFilter[i][j] = 0;
                complexRightFilter[i][j] = 0;
            }
            
            if (azimuths[i] < 0) {
                   azimuths[i] = -azimuths[i];
            }
            
            // recalculate the overlap part since the filter has been changed
            stereoConvolution(overlapInput, complexLeftFilter[i], complexRightFilter[i], outputLeft[i], outputRight[i], bufferSize, filterLength, 2 * bufferSize);
        
            // update the overlap part for the next iteration
            for (int j = 0; j < bufferSize; j++) {
                    overlapLeft[i][j] = outputLeft[i][j + bufferSize];
                    overlapRight[i][j] = outputRight[i][j + bufferSize];
            }
        }
    
        //doing convolution and get the main data for this iteration
        stereoConvolution(input, complexLeftFilter[i], complexRightFilter[i], outputLeft[i], outputRight[i], bufferSize, filterLength, 2 * bufferSize);
  
        //adding the overlap part with the main data
        for (int j = 0; j < bufferSize; j++) {
            ioDataLeft[j] +=  (short((outputLeft[i][j].re())/2 + (overlapLeft[i][j].re())/2))/myWorld->getNumObj();
            ioDataRight[j] += (short((outputRight[i][j].re())/2 + (overlapRight[i][j].re())/2))/myWorld->getNumObj();
        }
        
        //updating the overlapInput for the next iteration for the correpsonding obejct
        for (int j = 0; j < bufferSize * 2; j++) {
            if (j >= bufferSize) {
                overlapInput[j] = 0;
            } else {
                overlapInput[j] = input[j];
            }
        }

        // TODO: If the filter has been changed, didn't we already do this?
        //updating the default overlap information for the next iteration if the filter won't be changed
        for (int j = 0 ; j < bufferSize; j++) {
            overlapLeft[i][j]=outputLeft[i][j+bufferSize];
            overlapRight[i][j]=outputRight[i][j+bufferSize];
        }
    
        //storing the Azimuth value in this iteration for the comparison for the next iteration so that
        //we can know that whether the filter needs to be changed in the next iteration.
        prevAzimuths[i]=azimuths[i];
       
    }
}
