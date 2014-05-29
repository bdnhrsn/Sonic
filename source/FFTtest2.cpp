#include <iostream>
#include "../include/fft.h"
#include <math.h>
#include <stdio.h>

using namespace std;

//N is the FFT size, n1 is the size for the input, n2 is the size of the filter
complex *convolution(complex *input, complex *filter, int N, int n1, int n2);

int main()
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
	
	return 0;

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