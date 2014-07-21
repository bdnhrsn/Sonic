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
	
	//this is just used for testing
	nObj = 1;
	//this is just used for testing

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
	clFil = new complex[2 * bufferSize];
	crFil = new complex[2 * bufferSize];
    
    signFlag=0;//0 means the azimuth value is positive
    filterFlag=0;//0 means the filters for the current consecutive iterations are the same
    previousAzimuth = 190;//Used to store the azimuth value from the previous iteration
}


int Mixer3D::HRTFLoading(int* pAzimuth, int* pElevation, unsigned int samplerate, unsigned int diffused, complex *&leftFilter, complex *&rightFilter)
{
    //std::cout << "Entered HRTFLoading" << endl;

	int size = mit_hrtf_get(pAzimuth, pElevation, samplerate, diffused, lFil, rFil);


	for (int i = 0; i < size; i++)
	{
		leftFilter[i] = (double)(lFil[i]);
		rightFilter[i] = (double)(rFil[i]);
	}

	return size;
}

/*template <typename T>
void Mixer3D::write(std::ofstream& stream, const T& t)
{
	stream.write((const char*)&t, sizeof(T));
}

template <typename SampleType>
void Mixer3D::writeWAVData(const char* outFile, SampleType* buf, size_t bufSize, int sampleRate, short channels)
{
	std::ofstream stream(outFile, std::ios::binary);                // Open file stream at "outFile" location

	
	stream.write("RIFF", 4);                                        // sGroupID (RIFF = Resource Interchange File Format)
	write<int>(stream, 36 + bufSize);                               // dwFileLength
	stream.write("WAVE", 4);                                        // sRiffType

	
	stream.write("fmt ", 4);                                        // sGroupID (fmt = format)
	write<int>(stream, 16);                                         // Chunk size (of Format Chunk)
	write<short>(stream, 1);                                        // Format (1 = PCM)
	write<short>(stream, channels);                                 // Channels
	write<int>(stream, sampleRate);                                 // Sample Rate
	write<int>(stream, sampleRate * channels * sizeof(SampleType)); // Byterate
	write<short>(stream, channels * sizeof(SampleType));            // Frame size aka Block align
	write<short>(stream, 8 * sizeof(SampleType));                   // Bits per sample

	
	stream.write("data", 4);                                        // sGroupID (data)
	stream.write((const char*)&bufSize, 4);                         // Chunk size (of Data, and thus of dataSize)
	stream.write((const char*)buf, bufSize);                        // The samples DATA!!!
}*/

void Mixer3D::convolution(complex *input, complex *filter,complex *output, long nSig, long nFil, long nFFT)
{
	//Check for invalid inputs.
	if (input == NULL || filter == NULL)
	{
		cout << "Could not perform convolution on empty arrays!" << endl;
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

	complex *fInput=input, *fFilter=filter;

	//Perform FFT on both input and filter.
	CFFT::Forward(fInput, nFFT);
	CFFT::Forward(fFilter, nFFT);

	for (int i = 0; i < nFFT; i++)
		output[i] = fInput[i] * fFilter[i];

	CFFT::Inverse(output, nFFT);
	CFFT::Inverse(fInput, nFFT);
	CFFT::Inverse(fFilter, nFFT);


}
void Mixer3D::stereoConvolution(complex *input, complex *leftFilter, complex *rightFilter, complex *leftOutput, complex *rightOutput, long nSIG, long nFIL, long nFFT)
{
    
	convolution(input, leftFilter, leftOutput, nSIG, nFIL, nFFT);
	convolution(input, rightFilter, rightOutput, nSIG, nFIL, nFFT);

}

void Mixer3D::overlapConvolution(int Azimuth, int elevation, short *ioDataLeft,short *ioDataRight)
{
    signFlag=0;
    filterFlag=0;
    
		if (Azimuth < 0)
			signFlag = 1;
		else
			signFlag = 0;
    myWorld->getAudioObj(0)->fillAudioData(inputTempTemp1, bufferSize);
    

    for (int i = bufferSize; i < 2 * bufferSize; i++)
    {
        inputTempTemp1[i] = 0;
    }
    
   
    
    //judging whether the filter has changed
    if((Azimuth/5)!=(previousAzimuth/5))
    {
        filterFlag=1;
    }
    
    if(filterFlag)
    {
       
        //fetch the filter
        nTaps = HRTFLoading(&Azimuth, &elevation, sampleRate, 1, clFil, crFil);
       
        for (int i = nTaps; i < 2 * bufferSize; i++)
        {
            clFil[i] = 0;
            crFil[i] = 0;
        }
               if (signFlag)
            Azimuth = -Azimuth;
        stereoConvolution(overlapInput, clFil, crFil, outputLeft[0], outputRight[0], bufferSize, nTaps, 2 * bufferSize);
    //updating the overlap part for the next iteration
                for (int i = 0; i < bufferSize; i++)
        {
            overlapLeft[0][i] = outputLeft[0][i + bufferSize];
            overlapRight[0][i] = outputRight[0][i + bufferSize];
        }
        

    }
    
        //doing convolution and get the main data for this iteration
    stereoConvolution(inputTempTemp1, clFil, crFil, outputLeft[0], outputRight[0], bufferSize, nTaps, 2 * bufferSize);
  

    
    //adding the overlap part with the main data
   

    for (int i = 0; i < bufferSize; i++)
    {
       
        ioDataLeft[i] =   short((outputLeft[0][i].re())/2+ (overlapLeft[0][i].re())/2);
        ioDataRight[i] = short((outputRight[0][i].re())/2 + (overlapRight[0][i].re())/2);
    }
   
    //This is some unknown thing used for successful play of the ioData
    for (int i=0; i<0; i++){
        cout<<"In overlap function"<<endl;
    }
        
    //updating the overlap part for the next iteration
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
        overlapLeft[0][i]=outputLeft[0][i+bufferSize];
        overlapRight[0][i]=outputRight[0][i+bufferSize];
    }
    
    previousAzimuth=Azimuth;
   
}



//template void Mixer3D::write< short >(std::ofstream& stream, const short& t);
//template void Mixer3D::write< int >(std::ofstream& stream, const int& t);
//template void Mixer3D::writeWAVData<short>(const char* outFile, short* buf, size_t bufSize, int sampleRate, short channels);
