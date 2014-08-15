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
    
    signFlag=0;//0 means the azimuth value is positive
    filterFlag=0;//0 means the filters for the current consecutive iterations are the same
    previousAzimuth = new int[World::MAX_OBJ];//Used to store the azimuth value from the previous iteration
    Azimuth = new int[World::MAX_OBJ];
    elevation = new int[World::MAX_OBJ];
    for(int i =0; i<World::MAX_OBJ;i++)
    {
        previousAzimuth[i]=190;
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

	bool NFFTChanged = false;
	//If NFFT not a power of 2, or it is smaller than signal or filter, prompt for new.
	
	while (log2(nFFT) / log2(2) != (int)(log2(nFFT) / log2(2)) || nFFT < nSig || nFFT < nFil)
	{
		cout << "Please input a valid NFFT, which is >= nSig(" << nSig << ") and >= NFIL(" << nFil << ") : ";
		cin >> nFFT;
		NFFTChanged = true;
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
    for(int i = 0; i < bufferSize; i++)
    {
        ioDataLeft[i] = 0;
        ioDataRight[i] = 0;
    }
    
    for(int j = 0; j < myWorld->getNumObj() && myWorld->getAudioObj(j)->isActive() == true; j++)
    {
        
        Azimuth[j] = myWorld->getPlayer()->getAzimuth(myWorld->getAudioObj(j));
        signFlag = 0;
        filterFlag = 0;
    
		if (Azimuth[j] < 0)
			signFlag = 1;
		else
			signFlag = 0;
    
       if(!(myWorld->getAudioObj(j)->fillAudioData(inputTempTemp1, bufferSize)))
       {
            continue;
       }
        
        int amplitudeFactor = myWorld->getAudioObj(j)->getVolume()/myWorld->getPlayer()->getDistance(myWorld->getAudioObj(j)) ;
        
        for(int i = 0; i < bufferSize ; i++)
        {
            inputTempTemp1[i] *= amplitudeFactor;
        }
        
        for (int i = bufferSize; i < 2 * bufferSize; i++)
        {
            inputTempTemp1[i] = 0;
        }
    
   
    
        //judging whether the filter has changed
        if((Azimuth[j]/5)!=(previousAzimuth[j]/5))
        {
            filterFlag=1;
        }
       

        if(filterFlag)
        {
       
            //fetch the filter
            nTaps = HRTFLoading(&Azimuth[j], &elevation[j], sampleRate, 1, clFil[j], crFil[j]);
       
            for (int i = nTaps; i < 2 * bufferSize; i++)
            {
                clFil[j][i] = 0;
                crFil[j][i] = 0;
            }
               if (signFlag)
                   Azimuth[j] = -Azimuth[j];
            stereoConvolution(overlapInput, clFil[j], crFil[j], outputLeft[j], outputRight[j], bufferSize, nTaps, 2 * bufferSize);
                //updating the overlap part for the next iteration
                for (int i = 0; i < bufferSize; i++)
                {
                    overlapLeft[j][i] = outputLeft[j][i + bufferSize];
                    overlapRight[j][i] = outputRight[j][i + bufferSize];
                }
        

        }
    
            //doing convolution and get the main data for this iteration
        stereoConvolution(inputTempTemp1, clFil[j], crFil[j], outputLeft[j], outputRight[j], bufferSize, nTaps, 2 * bufferSize);
  
        //adding the overlap part with the main data
   

        for (int i = 0; i < bufferSize; i++)
        {
            
            ioDataLeft[i] +=  (short((outputLeft[j][i].re())/2+ (overlapLeft[j][i].re())/2))/myWorld->getNumObj();
            ioDataRight[i] += (short((outputRight[j][i].re())/2 + (overlapRight[j][i].re())/2))/myWorld->getNumObj();
            
            
        }
        
        for (int i = 0; i < bufferSize; i++)
        {
            overlapInput[i] = inputTempTemp1[i];
        }
        for (int i = bufferSize; i < 2 * bufferSize; i++)
        {
            overlapInput[i] = 0;
        }
    
        for(int i = 0 ; i < bufferSize; i++)
        {
            overlapLeft[j][i]=outputLeft[j][i+bufferSize];
            overlapRight[j][i]=outputRight[j][i+bufferSize];
        }
    
        previousAzimuth[j]=Azimuth[j];
       
    }
}
