#include<iostream>
#include"fft.h"
#include<math.h>
using namespace std;

int main()
{
	const double PI = atan(1.0)*4;
	cout << "hello world" << endl;
	const int N = 128;
	const int samplingRate = 256;

	//   Allocate memory for signal data
	complex *pSignal = new complex[N];
	
	cout << sin(PI) << endl;
	for (int n = 0; n < N; n++)
	{
		pSignal[n] = sin(2 * PI * 20 *n / samplingRate);
	}
	

	CFFT::Forward(pSignal, N);
	for (int n = 0; n < N; n++)
	{
		cout <<sqrt(pow((pSignal[n].re()),2)+pow((pSignal[n].im()),2)) << endl;
	}
	return 0;

}