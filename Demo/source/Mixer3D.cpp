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

Mixer3D::Mixer3D(int bufSize, int smpRate, int bitD, World *w)
{
	myWorld = w;
	bufferSize = bufSize;
	sampleRate = smpRate;
	bitDepth = bitD;
	//nObj = w->getNumActiveObjects();
	dataSize = 65536;
	
	//this is just used for testing
	nObj = 1;
	//this is just used for testing

	input = new complex*[World::MAX_OBJ];
	inputTemp = new complex*[World::MAX_OBJ];
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
		input[i] = new complex[dataSize*16];
		inputTemp[i] = new complex[dataSize*16];
		outputLeft[i] = new complex[2*bufferSize];
		outputRight[i] = new complex[2*bufferSize];
	}

	//This is for testing
	overlapInput = new complex[2 * bufferSize];


	/////This part is just for testing
	string inFile = "assets\\techtronic.wav";
	wavFileData inp;
	//LOAD THE WAV FILES
	//cout << "Attempting to load wav files..." << endl << endl;
	//input[0] = utility::loadCmpWavData(inFile, &inp.n, &inp.sampleRate, &inp.bitDepth, &inp.channels);
	//cout << "Wav files loading complete!" << endl;
	//////This part is just for testing

	
	
	cbResult = new long[2*dataSize*16];
	
	//Used to store the small chunk of output for every iteration
	cbResultSmall = new long[2 * bufferSize];
	
	//These are for filter fetch
	lFil = new short[2 * bufferSize];
	rFil = new short[2 * bufferSize];

	//These are for storing filters for processing
	clFil = new complex[2 * bufferSize];
	crFil = new complex[2 * bufferSize];
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

template <typename T>
void Mixer3D::write(std::ofstream& stream, const T& t)
{
	stream.write((const char*)&t, sizeof(T));
}

template <typename SampleType>
void Mixer3D::writeWAVData(const char* outFile, SampleType* buf, size_t bufSize, int sampleRate, short channels)
{
	std::ofstream stream(outFile, std::ios::binary);                // Open file stream at "outFile" location

	/* Header */
	stream.write("RIFF", 4);                                        // sGroupID (RIFF = Resource Interchange File Format)
	write<int>(stream, 36 + bufSize);                               // dwFileLength
	stream.write("WAVE", 4);                                        // sRiffType

	/* Format Chunk */
	stream.write("fmt ", 4);                                        // sGroupID (fmt = format)
	write<int>(stream, 16);                                         // Chunk size (of Format Chunk)
	write<short>(stream, 1);                                        // Format (1 = PCM)
	write<short>(stream, channels);                                 // Channels
	write<int>(stream, sampleRate);                                 // Sample Rate
	write<int>(stream, sampleRate * channels * sizeof(SampleType)); // Byterate
	write<short>(stream, channels * sizeof(SampleType));            // Frame size aka Block align
	write<short>(stream, 8 * sizeof(SampleType));                   // Bits per sample

	/* Data Chunk */
	stream.write("data", 4);                                        // sGroupID (data)
	stream.write((const char*)&bufSize, 4);                         // Chunk size (of Data, and thus of dataSize)
	stream.write((const char*)buf, bufSize);                        // The samples DATA!!!
}
complex *Mixer3D::getLeftFilter()
{
	return clFil;

}
void Mixer3D::convolution(complex *input, complex *filter,complex *output, long nSig, long nFil, long nFFT)
{
    //std::cout << "Entered convolution" << endl;
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
    //std::cout << "Entered stereoConvolution" << endl;
	convolution(input, leftFilter, leftOutput, nSIG, nFIL, nFFT);
	convolution(input, rightFilter, rightOutput, nSIG, nFIL, nFFT);

}
void Mixer3D::mix(short *ioDataLeft,short *ioDataRight) 
{
	

		
		/*int fullLength = 64 * bufferSize;//The whole length of the long file

		//loading the data in
		for (int i = 0; i < fullLength; i++)
		{
			inputTemp[0][i] = input[0][i % dataSize];
		}

		

		int Azimuth = 0;
		int elevation = -10;
		nTaps = HRTFLoading(&Azimuth, &elevation, sampleRate, 1, clFil, crFil);
		stereoConvolution(inputTemp[0], clFil, crFil, outputLeft[0], outputRight[0], fullLength, nTaps, fullLength);
		
		for (int i = 0; i < fullLength; i++)
		{
			cbResult[2 * i] = 0.5*outputLeft[0][i].re();
			cbResult[2 * i + 1] = 0.5*outputRight[0][i].re();
			

		}
		//ioDataLeft = cbResultLeft;
		//ioDataRight = cbResultRight;*/
    
    myWorld->getAudioObj(0)->fillAudioData(inputTemp[0], bufferSize);
    //myWorld->getAudioObj(1)->fillAudioData(inputTemp[1], bufferSize);
    int Azimuth = 60;
    int elevation = 0;
    nTaps = HRTFLoading(&Azimuth, &elevation, sampleRate, 1, clFil, crFil);
    stereoConvolution(inputTemp[0], clFil, crFil, outputLeft[0], outputRight[0], bufferSize, nTaps, bufferSize);
    
    for (int i = 0; i < bufferSize; i++)
    {
        //cbResult[2 * i] = outputLeft[0][i].re();
        //cbResultLeft[i] = (short)outputLeft[0][i].re();
        
        //ioDataLeft[i] = (short)outputLeft[0][i].re();
        //ioDataRight[i] = (short)outputRight[0][i].re();
        
        ioDataLeft[i]  = pow(2,15) * inputTemp[0][i].re() ;
        ioDataRight[i] = pow(2,15) * inputTemp[0][i].re() ;
        
        //ioDataLeft[i]  = pow(2,15) * ((inputTemp[0][i].re() + inputTemp[1][i].re())/2);
        //ioDataRight[i] = pow(2,15) * ((inputTemp[0][i].re() + inputTemp[1][i].re())/2);
        
        //cbResult[2 * i + 1] = outputRight[0][i].re();
        //cbResultRight[i] = (short)outputRight[0][i].re();
    }
    //ioDataLeft = cbResultLeft;
    //ioDataRight = cbResultRight;
    //begin[0] += bufferSize;
}

void Mixer3D::overlapConvolution(int Azimuth, int elevation, short *ioDataLeft,short *ioDataRight)
{
    //std::cout << "Entered overlapConvoltuion" << endl;
	bool flag;
	
		if (Azimuth < 0)
			flag = 1;
		else
			flag = 0;
    myWorld->getAudioObj(0)->fillAudioData(inputTempTemp1, bufferSize);
    
    //for (int i = 0; i < bufferSize; i++)
    //{
      //  inputTempTemp1[i] = input[0][i];
    //}
    
    //zero padding it
    for (int i = bufferSize; i < 2 * bufferSize; i++)
    {
        inputTempTemp1[i] = 0;
    }
    
    //fetch the filter
    nTaps = HRTFLoading(&Azimuth, &elevation, sampleRate, 1, clFil, crFil);
    for (int i = nTaps; i < 2 * bufferSize; i++)
    {
        clFil[i] = 0;
        crFil[i] = 0;
    }
    
    if (flag)
        Azimuth = -Azimuth;
    stereoConvolution(overlapInput, clFil, crFil, outputLeft[0], outputRight[0], bufferSize, nTaps, 2 * bufferSize);
    //updating the overlap part for the next iteration
    for (int i = 0; i < bufferSize; i++)
    {
        overlapLeft[0][i] = outputLeft[0][i + bufferSize];
        overlapRight[0][i] = outputRight[0][i + bufferSize];
    }
    
    
    stereoConvolution(inputTempTemp1, clFil, crFil, outputLeft[0], outputRight[0], bufferSize, nTaps, 2 * bufferSize);
    
    
    
    for (int i = 0; i < bufferSize; i++)
    {
        //cbResultSmall[2 * i] = outputLeft[0][i].re() + overlapLeft[0][i].re();
        //cbResultSmall[2 * i + 1] = outputRight[0][i].re() + overlapRight[0][i].re();
        ioDataLeft[i] =   short((outputLeft[0][i].re())/2+ (overlapLeft[0][i].re())/2);//overlapLeft[0][i].re();//
        ioDataRight[i] = short((outputRight[0][i].re())/2 + (overlapRight[0][i].re())/2);//overlapRight[0][i].re();//
        //ioDataLeft[i] = pow(2, 15) * inputTempTemp1[i].re();
        //ioDataRight[i] = pow(2, 15) * inputTempTemp1[i].re();
        
    }
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
    
}

/*void Mixer3D::stereoTest()
{
	
	int begin = -180;
	int end = 180;
	int step = 5;
	int testConstant = 70;
	int fullLength = (end-begin)/5 * bufferSize;//The whole length of the long file
	
	//loading the data in
	for (int i = 0; i < fullLength; i++)
	{
		inputTemp[0][i] = input[0][i];
	}
	
	const double PI = 3.14159;
	

	int elevation = -10;
	bool flag;
	for (int Azimuth = begin; Azimuth < end; Azimuth += step)
	{
		if (Azimuth < 0)
			flag = 1;
		else
			flag = 0;

		int n = ((Azimuth - begin) / 5)*(bufferSize);//the beginning index of the data in this iteration

		//loading the small chunk of the data which is going to be processed
		//This is where we calculate what overlap part we need to add
		
		if (n != 0)
		{
			for (int i = n - bufferSize; i < n; i++)
			{
				overlapInput[i%bufferSize] = inputTemp[0][i];
			}
			for (int i = bufferSize; i < 2 * bufferSize; i++)
			{
				overlapInput[i] = 0;
			}
		}



		for (int i = n; i < n + bufferSize; i++)
		{
			inputTempTemp1[i % (bufferSize)] = inputTemp[0][i];
		}

		//zero padding it
		for (int i = bufferSize; i < 2 * bufferSize; i++)
		{
			inputTempTemp1[i] = 0;
		}




		//fetch the filter
		nTaps = HRTFLoading(&testConstant, &elevation, sampleRate, 1, clFil, crFil);
		//zero padding the filters
		for (int i = nTaps; i < 2 * bufferSize; i++)
		{
			clFil[i] = 0;
			crFil[i] = 0;
		}



		if (flag)
			//Azimuth = -Azimuth;

		
		stereoConvolution(overlapInput, clFil, crFil, outputLeft[0], outputRight[0], bufferSize, nTaps, 2 * bufferSize);
		//updating the overlap part for the next iteration
		for (int i = 0; i < bufferSize; i++)
		{
			overlapLeft[0][i] = outputLeft[0][i + bufferSize];
			overlapRight[0][i] = outputRight[0][i + bufferSize];
		}


		//the FFT size needs to be 2*bufferSize because we need to preserve the overlap information
		//for the next iteration
		stereoConvolution(inputTempTemp1, clFil, crFil, outputLeft[0], outputRight[0], bufferSize, nTaps, 2 * bufferSize);
		//Zero padding needs to be redone after everytime after the convolution function
		//because both input and filter are overwritten

		//applying the overlap adding
		//This is probably the wrong overlapAdd we should apply
		for (int i = n; i < n + bufferSize; i++)
		{
			cbResult[2 * i] = outputLeft[0][i%bufferSize].re() + overlapLeft[0][i%bufferSize].re();
			cbResult[2 * i + 1] = outputRight[0][i%bufferSize].re() + overlapRight[0][i%bufferSize].re();
		}
		
	
	}

	//finding the maximum amount the cbResult and rescale it.
	
	double max = 0;
	for (int i = 0; i < 2 * fullLength; i++)
	{
		if (abs(cbResult[i])>max)
			max = abs(cbResult[i]);
	}
	cout <<"max is"<< max <<"and the limit is "<<pow(2,15)<< endl;

	for (int i = 0; i < 2 * fullLength; i++)
	{
		cbResult[i] = 0.9 * cbResult[i] * (pow(2, 15)/max);
	}	
}*/

template void Mixer3D::write< short >(std::ofstream& stream, const short& t);
template void Mixer3D::write< int >(std::ofstream& stream, const int& t);
template void Mixer3D::writeWAVData<short>(const char* outFile, short* buf, size_t bufSize, int sampleRate, short channels);
