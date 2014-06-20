#include <algorithm>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>

#include "../../include/Mixer3D.h"
#include "../../include/World.h"
#include "../../include/fft.h"
#include "../../include/wav.h"
#include "../../include/mit_hrtf_lib.h"
//
using namespace std;

struct wavFileData
{
	long n;
	int sampleRate;
	int bitDepth;
	int channels;
};


/*
	struct wavFileData
	{
		long n;
		int sampleRate;
		int bitDepth;
		int channels;
	};

	int HRTFLoading(int* pAzimuth, int* pElevation, unsigned int samplerate, unsigned int diffused, complex *&leftFilter, complex *&rightFilter)
	{
		int nTaps = 0;
		switch (samplerate)
		{
		case 44100:
			nTaps = MIT_HRTF_44_TAPS;
		case 48000:
			nTaps = MIT_HRTF_48_TAPS;
		case 88200:
			nTaps = MIT_HRTF_88_TAPS;
		case 96000:
			nTaps = MIT_HRTF_96_TAPS;
		}

		short* leftTemp = new short[nTaps];
		short* rightTemp = new short[nTaps];

		int size = mit_hrtf_get(pAzimuth, pElevation, samplerate, diffused, leftTemp, rightTemp);
		leftFilter = new complex[size];
		rightFilter = new complex[size];

		for (int i = 0; i < size; i++)
		{
			leftFilter[i] = (leftTemp[i]) / 1.0;
			rightFilter[i] = (rightTemp[i]) / 1.0;
		}

		return size;
	}


	template <typename T>
	void write(std::ofstream& stream, const T& t)
	{
		stream.write((const char*)&t, sizeof(T));
	}

	template <typename SampleType>
	void writeWAVData(const char* outFile, SampleType* buf, size_t bufSize, int sampleRate, short channels)
	{
		std::ofstream stream(outFile, std::ios::binary);                // Open file stream at "outFile" location

		//header
		stream.write("RIFF", 4);                                        // sGroupID (RIFF = Resource Interchange File Format)
		write<int>(stream, 36 + bufSize);                               // dwFileLength
		stream.write("WAVE", 4);                                        // sRiffType

		// Format Chunk 
		stream.write("fmt ", 4);                                        // sGroupID (fmt = format)
		write<int>(stream, 16);                                         // Chunk size (of Format Chunk)
		write<short>(stream, 1);                                        // Format (1 = PCM)
		write<short>(stream, channels);                                 // Channels
		write<int>(stream, sampleRate);                                 // Sample Rate
		write<int>(stream, sampleRate * channels * sizeof(SampleType)); // Byterate
		write<short>(stream, channels * sizeof(SampleType));            // Frame size aka Block align
		write<short>(stream, 8 * sizeof(SampleType));                   // Bits per sample

		// Data Chunk
		stream.write("data", 4);                                        // sGroupID (data)
		stream.write((const char*)&bufSize, 4);                         // Chunk size (of Data, and thus of bufferSize)
		stream.write((const char*)buf, bufSize);                        // The samples DATA!!!
	}


	int main()
	{
		const double PI = atan(1.0) * 4;

		string inFile = "assets\\input1mono.wav";

		complex *input = NULL, *filter = NULL;
		wavFileData inp, fil;

		//LOAD THE WAV FILES
		cout << "Attempting to load wav files..." << endl << endl;
		input = utility::loadCmpWavData(inFile, &inp.n, &inp.sampleRate, &inp.bitDepth, &inp.channels);
		cout << "Wav files loading complete!" << endl;

		cout << "Signal size is " << inp.n << endl;
		//cout << "Filter size is " << fil.n << endl;
		cout << "Sampling rate is " << inp.sampleRate << endl;

		complex *leftFilter = NULL, *rightFilter = NULL;

		int Azimuth = -90;
		int elevation = 0;
		cout << "Loading filter..." << endl;
		int filterSize = HRTFLoading(&Azimuth, &elevation, inp.sampleRate, 1, leftFilter, rightFilter);

		//Set up convolution output array.
		complex *output = NULL;
		long NFFT = pow(2, (int)(log(inp.n) / log(2)) + 1);

		//Perform convolution.
		cout << "Performing convolution..." << endl;
		output = CFFT::stereoConvMonoInputT(input, leftFilter, rightFilter, inp.n, filterSize, filterSize, NFFT);
		cout << "Convolution complete!\n";
		cout << "Number of sample values was: " << NFFT << endl;

		//Scale output from -1 to 1.
		int maxAmp = 1;
		for (long i = 0; i < NFFT; i++)
			if (output[i].re() > maxAmp)
				maxAmp = output[i].re();

		for (long i = 0; i < NFFT; i++)
		{
			output[i] /= maxAmp;
			output[i] *= .9;
		}

		//Output all real numbers of time domain to real.txt 
		CFFT::storingData(output, NFFT, "real0e90a.txt", 'r');

		//Output all wav data to output.wav
		short* wavData = (short*)malloc(sizeof(short)*NFFT);
		for (int i = 0; i < NFFT / 2; i++)
		{
			wavData[2 * i] = output[2 * i].re() * pow(2, 16 - 1);
			wavData[2 * i + 1] = output[2 * i + 1].re() * pow(2, 16 - 1);
		}

		string s = "output0e-45a.wav";
		writeWAVData(s.c_str(), wavData, NFFT, inp.sampleRate, 2);

		delete input;
		delete filter;

		return 0;
	}
*/

int main()
{
	string inFile = "assets\\input1mono.wav";
	wavFileData inp;
	//LOAD THE WAV FILES
	cout << "Attempting to load wav files..." << endl << endl;
	complex* temp = utility::loadCmpWavData(inFile, &inp.n, &inp.sampleRate, &inp.bitDepth, &inp.channels);
	int sampleRate = inp.sampleRate;
	int bufferSize = 65536*8;
	int bitDepth = 16;
	int diffuse = 1;
	Player playerA = Player();
	AudioObj AudioObjA = AudioObj();
	World myWorld = World();
	myWorld.addAudioObj();
	Mixer3D myMixer = Mixer3D(bufferSize,sampleRate,bitDepth,&myWorld);
	short* left = new short[bufferSize], *right = new short[bufferSize];
	
	//myMixer.mix(left,right);
	myMixer.stereoTest();
	temp = myMixer.getLeftFilter();
	short *temp2;
	temp2 = myMixer.getTemp();
	string s = "output0e-45a.wav";
	myMixer.writeWAVData(s.c_str(), temp2, 4*bufferSize, inp.sampleRate, 2);
	

	//testing
 	return 0;

}