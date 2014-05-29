#include<iostream>
#include"fft.h"
#include<math.h>
using namespace std;

//N is the FFT size, n1 is the size for the input, n2 is the size of the filter
//complex *convolution(complex *input, complex *filter, int N, int n1, int n2);

int main()
{
	const double PI = atan(1.0)*4;
	cout << "hello world" << endl;
	int inputSize, filterSize;
	int N;
	const int samplingRate = 256;
	cout << "the input size is (samples)";
	cin >> inputSize ;
	cout << "the filter size is (samples)";
	cin >> filterSize;
	cout << "the FFT size is (samples)";
	cin >> N;
	
	while (inputSize>N || filterSize>N)
	{
		cout << "N must be larger both n1 and n2 so please enter N again";
		cin >> N;
		cout << "N is " << N;
	}
	//   Allocate memory for signal data
	complex *pSignal = new complex[N];
	complex *filter = new complex[N];
	complex *output = new complex[N];

	//input
	for (int n = 0; n < inputSize; n++)
	{
		if (n < inputSize)
		{pSignal[n] = sin(2 * PI * 20 * n / samplingRate); }
	//	cout <<"input["<<n<<"] "<< pSignal[n].re()<<endl;
	}
	
	cout << endl << endl << endl;

	//filter
	for (int n = 0; n < N; n++)
	{
		if (n<filterSize)
		filter[n] = sin(2 * PI * 20 * n / samplingRate);
		//cout<<n <<"      filter["<<n<<"] "<< filter[n].re()<<endl;
	}
	

	output = CFFT::convolution(pSignal, filter, N, inputSize, filterSize);
	
	
	for (int n = 0; n < N; n++)
	{
		//cout << "Real: " << output[n].re() << " Imag: " << output[n].im() << endl;
		cout<<"the output is " << sqrt(output[n].norm()) << "    "<<n<<endl;
	}
	
	
	return 0;

}

/*
//N is the FFT size, n1 is the size for the input, n2 is the size of the filter
//The memory allocated to input and output should both be N with certain amount of zeros
//appended after the valid data point
complex *convolution(complex *input, complex *filter,int N,int n1, int n2)
{
	
	//store the fft for the input data
	complex *temp1 = new complex[N];
	
	//store the fft for the filter data
	complex *temp2 = new complex[N];
	//store the output data
	complex *temp3 = new complex[N];

	CFFT::Forward(input,temp1, N);
	CFFT::Forward(filter,temp2, N);

	for (int i = 0; i < N; i++)
	{
		//cout<<i << "    the input is " << sqrt(temp1[i].norm()) << "the filter is " << sqrt(temp2[i].norm());
		temp3[i] = temp1[i] * temp2[i];
		//cout << "and the output fft is" << sqrt(temp3[i].norm()) << endl;
	}
	CFFT::Inverse(temp3, N);
	for (int i = 0; i < N; i++)
	{
		cout<<i << "    the input is " << sqrt(temp1[i].norm()) << "the filter is " << sqrt(temp2[i].norm());
		cout << "and the output fft is" << sqrt(temp3[i].norm()) << endl;
	}
	return temp3;
}
*/