#include <iostream>
#include "../include/fft.h"
#include <math.h>
#include <stdio.h>

using namespace std;

//N is the FFT size, n1 is the size for the input, n2 is the size of the filter
complex *convolution(complex *input, complex *filter, int N, int n1, int n2);
void convolution(complex *input, complex *filter, complex *output, int NSIG, int NFIL, int &NFFT); 
void ffttest();

int main()
{
	int NSIG, NFIL, NFFT;
	const int samplingRate = 256;
	const double PI = atan(1.0)*4;

	cout << "The input size is (in samples): ";
	cin >> NSIG ;
	cout << "The filter size is (in samples): ";
	cin >> NFIL;	
	cout << "The FFT size is (in samples): ";
	cin >> NFFT;

	complex *input = new complex[NSIG];
	complex *filter = new complex[NFIL];
	complex *output = NULL;

	for(int i = 0; i < NSIG; i++)
	{
		input[i] = sin(2 * PI * 20 * ((float)i)/samplingRate);
		cout << "Input " << i << " is " << sqrt(input[i].norm()) << endl;
	}
	for(int i = 0; i < NFIL; i++)
	{
		filter[i] = sin(2 * PI * 20 * ((float)i)/samplingRate);
		cout << "Input " << i << " is " << sqrt(filter[i].norm()) << endl;
	}

	convolution(input, filter, output, NSIG, NFIL, NFFT);
	for(int i = 0; i < NFFT; i++)
	{
		cout << "Output " << i << " is " << sqrt(output[i].norm()) << endl;
	}

	return 0;
}

void ffttest()
{
	int NSIG, NFFT, NMAX;
	const int samplingRate = 256;
	const double PI = atan(1.0)*4;

	cout << "The input size is (in samples): ";
	cin >> NSIG ;
	cout << "The FFT size is (in samples): ";
	cin >> NFFT;
	NMAX = max(NSIG, NFFT);

	//   Allocate memory for signal data
	complex *input = new complex[NMAX];
	complex *output = new complex[NMAX];

	for(int i = 0; i < NSIG; i++)
		input[i] = sin(2 * PI * 20 * ((float)i)/samplingRate);
	for(int i = 0; i < NMAX; i++)
		printf("Sample %d is value: %f\n", i, input[i]);
	cout << endl << endl;

	CFFT::Forward(input, output, NFFT);
	for(int i = 0; i < NMAX; i++)
		printf("Output %d is value: %f\n", i, sqrt(output[i].norm()));
}

//N is the FFT size, n1 is the size for the input, n2 is the size of the filter
complex *convolution(complex *input, complex *filter, int N, int n1, int n2)
{
	while (n1 > N || n2 > N)
	{
		cout << "N must be larger both n1 and n2 so please enter N again";
		cin >> N;
		cout << "N is " << N;
	}

	complex *temp1 = input;
	complex *temp2 = filter;
	if (n1 > n2)
	{
		for (int n = n2; n < n1; n++)
			temp2[n] = 0;
		for (int n = 0; n < n1; n++)
			cout << "input is " << temp1[n].re() << "  filter is" << temp2[n].re() << endl;
	}
	else
	{
		for (int n = n1; n < n2; n++)
			temp1[n] = 0;
		for (int n = 0; n < n2; n++)
			cout << "input is " << temp1[n].re() << "  filter is" << temp2[n].re() << endl;
	}
	complex *temp3= new complex[N];

	CFFT::Forward(temp1, N);
	CFFT::Forward(temp2, N);
	for (int i = 0; i < N; i++)
	{
		temp3[i] = temp1[i] * temp2[i];
	}

	//CFFT::Inverse(temp3, N);
	return temp3;
}

//Performs convolution of input and filter. input and filter are automatically zero padded, but NFFT must be equal or greater than both.
//The output pointer will be assigned to a new complex array in the case it is NULL or the NFFT size was invalid.
void convolution(complex *input, complex *filter, complex *output, int NSIG, int NFIL, int &NFFT)
{
	//Check for invalid inputs.
	if(input == NULL || filter == NULL)
	{
		cout << "Could not perform convolution on empty arrays!" << endl;
		return;
	}

	bool NFFTChanged = false;
	while(NFFT < NSIG || NFFT < NFIL)
	{
		cout << "Please input a valid NFFT, which is >= NSIG(" << NSIG << ") and >= NFIL(" << NFIL <<") : ";
		cin >> NFFT;
		NFFTChanged = true;
	}

	//Perform zero padding.
	complex *pInput, *pFilter;

	if(NSIG < NFFT)
	{
		pInput = new complex[NFFT];
		for(int i = 0; i < NSIG; i++)
			pInput[i] = input[i];
	}
	else
		pInput = input;

	if(NFIL < NFFT)
	{
		pFilter = new complex[NFFT];
		for(int i = 0; i < NFIL; i++)
			pFilter[i] = filter[i];
	}
	else
		pFilter = filter;
	
	//Perform convolution.
	cout << "Ready for convolution!\n";
	if(NFFTChanged || output == NULL)
		output = new complex[NFFT];
	
	CFFT::Forward(pInput, NFFT);
	CFFT::Forward(pFilter, NFFT);
	cout << "Performed FFTs!\n";

	for(int i = 0; i < NFFT; i++)
		output[i] = pInput[i] * pFilter[i];
}