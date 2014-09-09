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


struct wavFileData
{
	long n;
	int sampleRate;
	int bitDepth;
	int channels;
};

Mixer3D::Mixer3D(int bufSize, int smpRate, int bitD, World *w):myWorld(w), bufferSize(bufSize), sampleRate(smpRate), bitDepth(bitD)
{
	outputLeft = new complex*[World::MAX_OBJ];
	outputRight = new complex*[World::MAX_OBJ];
	overlapLeft = new complex*[World::MAX_OBJ];
	overlapRight = new complex*[World::MAX_OBJ];
	
	//This is the variable used to store the data gotten from the world
	inputTempTemp1 = new complex[2*bufferSize];

	for (int i = 0; i < World::MAX_OBJ; i++)
	{
		overlapLeft[i] = new complex[bufferSize];
		overlapRight[i] = new complex[bufferSize];
		outputLeft[i] = new complex[2*bufferSize];
		outputRight[i] = new complex[2*bufferSize];
	}

	//This is for testing
	overlapInput = new complex[2 * bufferSize];

	//These are for filter fetch
	lFil = new short[2 * bufferSize];
	rFil = new short[2 * bufferSize];

	//These are for storing filters for processing
    clFil = new complex*[World::MAX_OBJ];
    crFil = new complex*[World::MAX_OBJ];
    for(int i = 0; i < World::MAX_OBJ; i++)
    {
        clFil[i] = new complex[2*bufferSize];
        crFil[i] = new complex[2*bufferSize];
    }
    
    fInput = new complex[2 * bufferSize];
    fFilter = new complex[2 * bufferSize];
    
    Azimuths = new int[World::MAX_OBJ];
    elevations = new int[World::MAX_OBJ];
    prevAzimuths = new int[World::MAX_OBJ];
    prevElevations = new int[World::MAX_OBJ];
    
    updateAngles();
    
}

void Mixer3D::updateAngles() {
    Player *p1 = myWorld->getPlayer();
    for (int i = 0; i < myWorld->getNumObj(); i++) {
        AudioObj* iAudioObj = myWorld->getAudioObj(i);
        Azimuths[i] = p1->computeAzimuth(iAudioObj);
        elevations[i] = p1->computeElevation(iAudioObj);
        printf("Azimuths[%d]: %d\n", i, Azimuths[i]);
        printf("elevations[%d]: %d\n", i, elevations[i]);
    }
}

int Mixer3D::HRTFLoading(int* pAzimuth, int* pElevation, unsigned int samplerate, unsigned int diffused, complex *&leftFilter, complex *&rightFilter)
{
    int size = mit_hrtf_get(pAzimuth, pElevation, samplerate, diffused, lFil, rFil);


	for (int i = 0; i < size; i++)
	{
		leftFilter[i] = (double)(lFil[i]);
		rightFilter[i] = (double)(rFil[i]);
	}

	return size;
}


void Mixer3D::convolution(complex *input, complex *filter,complex *output, long nSig, long nFil, long nFFT)
{
	//Check for invalid inputs.
	if (input == NULL || filter == NULL)
	{
		cout << "Could not perform convolution on empty aaaa arrays!" << endl;
		return;
	}

	// If NFFT not a power of 2, or it is smaller than signal or filter, prompt for new.
	while (log2(nFFT) / log2(2) != (int)(log2(nFFT) / log2(2)) || nFFT < nSig || nFFT < nFil)
	{
		cout << "Please input a valid NFFT, which is >= nSig(" << nSig << ") and >= NFIL(" << nFil << ") : ";
		cin >> nFFT;
	}

	//Perform FFT on both input and filter.
	CFFT::Forward(input,fInput, (unsigned int)nFFT);
	CFFT::Forward(filter,fFilter, (unsigned int)nFFT);

	for (int i = 0; i < nFFT; i++)
		output[i] = fInput[i] * fFilter[i];

	CFFT::Inverse(output, (unsigned int)nFFT);
}

void Mixer3D::stereoConvolution(complex *input, complex *leftFilter, complex *rightFilter, complex *leftOutput, complex *rightOutput, long nSIG, long nFIL, long nFFT)
{
	convolution(input, leftFilter, leftOutput, nSIG, nFIL, nFFT);
	convolution(input, rightFilter, rightOutput, nSIG, nFIL, nFFT);
}

void Mixer3D::overlapConvolution( short *ioDataLeft,short *ioDataRight)
{
    // Clearing the left and right channels
    for(int i = 0; i < bufferSize; i++) {
        ioDataLeft[i] = 0;
        ioDataRight[i] = 0;
    }
    
    //looping through the audioObjects and do the mixing one sequentially
    
    // TODO: Possible bug? Cuts short if one audioObj is inactive?
    for (int i = 0; i < myWorld->getNumObj() && myWorld->getAudioObj(i)->isActive() == true; i++) {
        // dynamically calculate the Azimuth and Elevation between every object and the player
        updateAngles();
        AudioObj* iAudioObj = myWorld->getAudioObj(i);
        
        // loading in input data for the iteration accordingly
       if (!(iAudioObj->fillAudioData(inputTempTemp1, bufferSize))) {
            continue;
       }
        
        // scale volume according to distance of object from player
        // TODO: Is this realistic?
        float iVolume = iAudioObj->getVolume();
        float iDistance = myWorld->getPlayer()->computeDistanceFrom(iAudioObj) ;
        float iAmplitudeFactor = iVolume / iDistance;
        for(int j = 0; j < bufferSize ; j++) {
            inputTempTemp1[j] *= iAmplitudeFactor;
        }
        
        //zero padding the input for this iteration for the proper fft size
        for (int j = bufferSize; j < 2 * bufferSize; j++) {
            inputTempTemp1[j] = 0;
        }

        if (Azimuths[i] != prevAzimuths[i] ||
           elevations[i] != prevElevations[i]) {
            // object location relative to player has changed, so fetch a new filter
            nTaps = HRTFLoading(&Azimuths[i], &elevations[i], sampleRate, 1, clFil[i], crFil[i]);
            
            //zeroing padding it
            for (int j = nTaps; j < 2 * bufferSize; j++) {
                clFil[i][j] = 0;
                crFil[i][j] = 0;
            }
            
            if (Azimuths[i] < 0) {
                   Azimuths[i] = -Azimuths[i];
            }
            
            // recalculate the overlap part since the filter has been changed
            stereoConvolution(overlapInput, clFil[i], crFil[i], outputLeft[i], outputRight[i], bufferSize, nTaps, 2 * bufferSize);
        
            // update the overlap part for the next iteration
            for (int j = 0; j < bufferSize; j++) {
                    overlapLeft[i][j] = outputLeft[i][j + bufferSize];
                    overlapRight[i][j] = outputRight[i][j + bufferSize];
            }
        }
    
        //things needed to be done no matter whether the filter has been changed or not
        //doing convolution and get the main data for this iteration
        stereoConvolution(inputTempTemp1, clFil[i], crFil[i], outputLeft[i], outputRight[i], bufferSize, nTaps, 2 * bufferSize);
  
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
                overlapInput[j] = inputTempTemp1[j];
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
        prevAzimuths[i]=Azimuths[i];
       
    }
}
