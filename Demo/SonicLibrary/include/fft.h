//   fft.h - declaration of class
//   of fast Fourier transform - FFT
//
//   The code is property of LIBROW
//   You can use it on your own
//   When utilizing credit LIBROW site

#ifndef _FFT_H_
#define _FFT_H_

#include "complex.h"
#include "string.h"

using namespace std;

class CFFT
{
	public:
		//Convolution functions
		//NFFT is the FFT size (will be modified if invalid!), nSIG is the size for the input, NFIL is the size of the filter
		//Zero padding is automatically taken care of in the convolution function.
		//T means the function returns result in time domain; F is the result in the frequency domain.
		static complex* convolutionF(const complex *input,const complex *filter, long nSIG, long NFIL, long &NFFT);
		static complex* convolutionT(const complex *input,const complex *filter, long nSIG, long NFIL, long &NFFT);
		static complex* stereoConvMonoInputF(const complex *input, const complex *filterLeft, const complex *filterRight, long nSIG, long NFILL, long NFILR, long &NFFT);
		static complex* stereoConvMonoInputT(const complex *input, const complex *filterLeft, const complex *filterRight, long nSIG,long NFILL, long NFILR, long &NFFT);
		static complex* stereoConvStereoInputF(const complex *input, const complex *filterLeft, const complex *filterRight, long nSIG, long NFILL, long NFILR, long &NFFT);
		static complex* stereoConvStereoInputT(const complex *input, const complex *filterLeft, const complex *filterRight, long nSIG, long NFILL, long NFILR, long &NFFT);
		
		
		//storing the an array into a text file
		//filename is the file name you want to store the data into
		//datatype represents the data you wanna store: real/real+imag/amplitude
    static void storingData(complex *data, int NFFT, std::string temp, char datatype);
		
		//   FORWARD FOURIER TRANSFORM
		//     Input  - input data
		//     Output - transform result
		//     N      - length of both input data and result
		static bool Forward(const complex *const Input, complex *const Output, const unsigned int N);
	
		//   FORWARD FOURIER TRANSFORM, INPLACE VERSION
		//     Data - both input data and output
		//     N    - length of input data
		static bool Forward(complex *const Data, const unsigned int N);
	
		//   INVERSE FOURIER TRANSFORM
		//     Input  - input data
		//     Output - transform result
		//     N      - length of both input data and result
		//     Scale  - if to scale result
		static bool Inverse(const complex *const Input, complex *const Output, const unsigned int N, const bool Scale = true);
	
		//   INVERSE FOURIER TRANSFORM, INPLACE VERSION
		//     Data  - both input data and output
		//     N     - length of both input data and result
		//     Scale - if to scale result
		static bool Inverse(complex *const Data, const unsigned int N, const bool Scale = true);

	protected:
		//   Rearrange function and its inplace version
		static void Rearrange(const complex *const Input, complex *const Output, const unsigned int N);

		static void Rearrange(complex *Data, const unsigned int N);

	
	
		//   FFT implementation
		static void Perform(complex *const Data, const unsigned int N, const bool Inverse = false);
	
		//   Scaling of inverse FFT result
		static void Scale(complex *const Data, const unsigned int N);
};

#endif
