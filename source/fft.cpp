//   fft.cpp - impelementation of class
//   of fast Fourier transform - FFT
//
//   The code is property of LIBROW
//   You can use it on your own
//   When utilizing credit LIBROW site

#include <fstream>
#include <iostream>
#include <math.h>
#include "../include/fft.h"

using namespace std;

//Convolution function which returns the frequency representation of the result.
//NFFT is the FFT size, nSIG is the size for the input, NFIL is the size of the filter.
complex* CFFT::convolutionF(const complex *input,const complex *filter, long nSIG, long NFIL, long &NFFT)
{
	//Check for invalid inputs.
	if(input == NULL || filter == NULL)
	{
		cout << "Could not perform convolution on empty arrays!" << endl;
		return NULL;
	}

    //TODO: Do we need this error checking? We're already doing this in Mixer3D::convolution.
	//If NFFT not a power of 2, or it is smaller than signal or filter, prompt for new.
	while (log(NFFT) / log(2) != (int)(log(NFFT) / log(2)) || NFFT < nSIG || NFFT < NFIL)
	{
		cout << "Please input a valid NFFT, which is >= nSIG(" << nSIG << ") and >= NFIL(" << NFIL <<") : ";
		cin >> NFFT;
	}

	//Perform zero padding.
	complex *fInput, *fFilter;

    // TODO: We're not zero-padding here, and these
    // shouldn't be declared on the heap
	fInput = new complex[NFFT];
	for(int i = 0; i < nSIG; i++)
		fInput[i] = input[i];

	fFilter = new complex[NFFT];
	for(int i = 0; i < NFIL; i++)
		fFilter[i] = filter[i];
	
	//Store the output data.
	complex *output = new complex[NFFT];
	
	//Perform FFT on both input and filter.
	CFFT::Forward(fInput, (unsigned int)NFFT);
	CFFT::Forward(fFilter, (unsigned int)NFFT);

	for(int i = 0; i < NFFT; i++)
		output[i] = fInput[i] * fFilter[i];

    //TODO: Caller is responsible for deleting output
	return output;
}

//Convolution function which returns the time representation of the result.
//NFFT is the FFT size, nSIG is the size for the input, NFIL is the size of the filter.
complex* CFFT::convolutionT(const complex *input,const complex *filter, long nSIG, long NFIL, long &NFFT)
{
	//Store the output data.
	complex *output = convolutionF(input, filter, nSIG, NFIL, NFFT);
	
	//Perform IFFT on the ouput.
	CFFT::Inverse(output, (unsigned int)NFFT);
	
	return output;
}

complex* CFFT::stereoConvMonoInputF(const complex *input,const complex *filterLeft,const complex *filterRight, long nSIG, long NFILL, long NFILR, long &NFFT)
{
	complex *result = stereoConvMonoInputT(input, filterLeft, filterRight, nSIG, NFILL, NFILR, NFFT);
	CFFT::Forward(result, (unsigned int)NFFT);
	return result;
}

//The size of the ouput will be 2 times of the size of FFT for the input signal and the NFFT value
//will be doubled after running the function
complex* CFFT::stereoConvMonoInputT(const complex *input,const complex *filterLeft,const complex *filterRight, long nSIG, long NFILL, long NFilR, long &NFFT)
{
	complex *tempLeft = new complex[NFFT];
	complex *tempRight = new complex[NFFT];
	complex *result = new complex[2*NFFT];
	
	tempLeft = CFFT::convolutionT(input, filterLeft, nSIG, NFILL, NFFT);
	tempRight = CFFT::convolutionT(input, filterRight, nSIG, NFILL, NFFT);
	NFFT = NFFT * 2;
	for (int i = 0; i < NFFT / 2; i++)
	{
		result[2 * i] = tempLeft[i];
		result[2 * i + 1] = tempRight[i];
	}

	delete tempLeft;
	delete tempRight;

    // TODO: Caller is responsible for deleting result	
	return result;
}

/*
complex* CFFT::stereoConvStereoInputT(const complex *input, const complex *filterLeft, const complex *filterRight, long nSIG, long NFILL, long NFILR, long &NFFT)
{
	NFFT = NFFT / 2;
	complex *leftTemp = new complex[NFFT];
	complex *rightTemp = new complex[NFFT];

	

	

	return 0;

}

*/


//storing the an array into a text file
//filename is the file name you want to store the data into
//datatype represents the data you wanna store: real/real+imag/amplitude
void CFFT::storingData(complex *data, int NFFT,string temp ,char datatype)
{
	//string temp= filename;
	ofstream outputFile(temp.c_str());
	if (outputFile.is_open())
	{
		switch (datatype)
		{
		case 'r':
			//ofstream outputFile("real.txt");
			for (int i = 0; i < NFFT; i++)
				outputFile << data[i].re() << endl;
			break;

		case 'c':
			for (int i = 0; i < NFFT; i++)
				outputFile << data[i].re() << "    " << data[i].im() << endl;
			break;
		case 'a':
			for (int i = 0; i < NFFT; i++)
				outputFile << sqrt(data[i].norm()) << endl;
			break;
		}
		outputFile.close();
	}
}



//   FORWARD FOURIER TRANSFORM
//     Input  - input data
//     Output - transform result
//     N      - length of both input data and result
bool CFFT::Forward(const complex *const Input, complex *const Output, const unsigned int N)
{
	//   Check input parameters
	if (!Input || !Output || N < 1 || N & (N - 1))
		return false;
	//   Initialize data
	Rearrange(Input, Output, N);
	//   Call FFT implementation
	Perform(Output, N);
	//   Succeeded
	return true;
}

//   FORWARD FOURIER TRANSFORM, INPLACE VERSION
//     Data - both input data and output
//     N    - length of input data
bool CFFT::Forward(complex *const Data, const unsigned int N)
{
	//   Check input parameters
	if (!Data || N < 1 || N & (N - 1))
		return false;
	//   Rearrange
	Rearrange(Data, N);
	//   Call FFT implementation
	Perform(Data, N);
	//   Succeeded
	return true;
}

//   INVERSE FOURIER TRANSFORM
//     Input  - input data
//     Output - transform result
//     N      - length of both input data and result
//     Scale  - if to scale result
bool CFFT::Inverse(const complex *const Input, complex *const Output, const unsigned int N, const bool Scale /* = true */)
{
	//   Check input parameters
	if (!Input || !Output || N < 1 || N & (N - 1))
		return false;
	//   Initialize data
	Rearrange(Input, Output, N);
	//   Call FFT implementation
	Perform(Output, N, true);
	//   Scale if necessary
	if (Scale)
		CFFT::Scale(Output, N);
	//   Succeeded
	return true;
}

//   INVERSE FOURIER TRANSFORM, INPLACE VERSION
//     Data  - both input data and output
//     N     - length of both input data and result
//     Scale - if to scale result
bool CFFT::Inverse(complex *const Data, const unsigned int N, const bool Scale /* = true */)
{
	//   Check input parameters
	if (!Data || N < 1 || N & (N - 1))
		return false;
	//   Rearrange
	Rearrange(Data, N);
	//   Call FFT implementation
	Perform(Data, N, true);
	//   Scale if necessary
	if (Scale)
		CFFT::Scale(Data, N);
	//   Succeeded
	return true;
}

//   Rearrange function
void CFFT::Rearrange(const complex *const Input, complex *const Output, const unsigned int N)
{
	//   Data entry position
	unsigned int Target = 0;
	//   Process all positions of input signal
	for (unsigned int Position = 0; Position < N; ++Position)
	{
		//  Set data entry
		Output[Target] = Input[Position];
		//   Bit mask
		unsigned int Mask = N;
		//   While bit is set
		while (Target & (Mask >>= 1))
			//   Drop bit
			Target &= ~Mask;
		//   The current bit is 0 - set it
		Target |= Mask;
	}
}

//   Inplace version of rearrange function

void CFFT::Rearrange(complex *const Data, const unsigned int N)
{
	//   Swap position
	unsigned int Target = 0;
	//   Process all positions of input signal
	for (unsigned int Position = 0; Position < N; ++Position)
	{
		//   Only for not yet swapped entries
		if (Target > Position)
		{
			//   Swap entries
			const complex Temp(Data[Target]);
			Data[Target] = Data[Position];
			Data[Position] = Temp;
		}
		//   Bit mask
		unsigned int Mask = N;
		//   While bit is set
		while (Target & (Mask >>= 1))
			//   Drop bit
			Target &= ~Mask;
		//   The current bit is 0 - set it
		Target |= Mask;
	}
}

//   FFT implementation
void CFFT::Perform(complex *const Data, const unsigned int N, const bool Inverse /* = false */)
{
	const double pi = Inverse ? 3.14159265358979323846 : -3.14159265358979323846;
	//   Iteration through dyads, quadruples, octads and so on...
	for (unsigned int Step = 1; Step < N; Step <<= 1)
	{
		//   Jump to the next entry of the same transform factor
		const unsigned int Jump = Step << 1;
		//   Angle increment
		const double delta = pi / double(Step);
		//   Auxiliary sin(delta / 2)
		const double Sine = sin(delta * .5);
		//   Multiplier for trigonometric recurrence
		const complex Multiplier(-2. * Sine * Sine, sin(delta));
		//   Start value for transform factor, fi = 0
		complex Factor(1.);
		//   Iteration through groups of different transform factor
		for (unsigned int Group = 0; Group < Step; ++Group)
		{
			//   Iteration within group 
			for (unsigned int Pair = Group; Pair < N; Pair += Jump)
			{
				//   Match position
				const unsigned int Match = Pair + Step;
				//   Second term of two-point transform
				const complex Product(Factor * Data[Match]);
				//   Transform for fi + pi
				Data[Match] = Data[Pair] - Product;
				//   Transform for fi
				Data[Pair] += Product;
			}
			//   Successive transform factor via trigonometric recurrence
			Factor = Multiplier * Factor + Factor;
		}
	}
}

//   Scaling of inverse FFT result
void CFFT::Scale(complex *const Data, const unsigned int N)
{
	const double Factor = 1. / double(N);
	//   Scale all data entries
	for (unsigned int Position = 0; Position < N; ++Position)
		Data[Position] *= Factor;
}
