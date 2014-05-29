//   fft.h - declaration of class
//   of fast Fourier transform - FFT
//
//   The code is property of LIBROW
//   You can use it on your own
//   When utilizing credit LIBROW site

#ifndef _FFT_H_
#define _FFT_H_

//   Include complex numbers header
#include "complex.h"

class CFFT
{
public:
	
	//convolution function
	//N is the FFT size, n1 is the size for the input, n2 is the size of the filter
	//The memory allocated to input and output should both be N with certain amount of zeros
	//appended after the valid data point
	static complex *convolution(complex *input, complex *filter, int N, int n1, int n2);


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
	static void Rearrange(complex *const Data, const unsigned int N);

	//   FFT implementation
	static void Perform(complex *const Data, const unsigned int N, const bool Inverse = false);

	//   Scaling of inverse FFT result
	static void Scale(complex *const Data, const unsigned int N);
};

#endif
