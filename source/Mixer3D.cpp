#include "../include/Mixer3D.h"	
#include "../include/mit_hrtf_lib.h"

Mixer3D::Mixer3D(int bufSize, int smpRate, int bitD, World* w)
{
	myWorld = w;
	bufferSize = bufSize;
	sampleRate = smpRate;
	bitDepth = bitD;

	input = new complex[bufferSize];

	output = new complex*[World::MAX_OBJ];
	for (int i = 0; i < World::MAX_OBJ; i++)
		output[i] = new complex[bufferSize * 2];

	result = new complex[bufferSize * 2];
	cbResult = new short[bufferSize * 2];
	
	switch (smpRate)
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
	lFil = new short[nTaps];
	rFil = new short[nTaps];
	clFil = new complex[nTaps];
	crFil = new complex[nTaps];

}

int Mixer3D::HRTFLoading(int* pAzimuth, int* pElevation, unsigned int samplerate, unsigned int diffused, complex *&leftFilter, complex *&rightFilter)
{

	int size = mit_hrtf_get(pAzimuth, pElevation, samplerate, diffused, &lFil, &rFil);


	for (int i = 0; i < size; i++)
	{
		leftFilter[i] = (lFil[i]) / 1.0;
		rightFilter[i] = (rFil[i]) / 1.0;
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
	stream.write((const char*)&bufSize, 4);                         // Chunk size (of Data, and thus of bufferSize)
	stream.write((const char*)buf, bufSize);                        // The samples DATA!!!
}

void Mixer3D::mix()
{
	int Azimuth = 50;
	int elevation = 0;
	HRTFLoading(&Azimuth, &elevation, sampleRate, 1, clFil, crFil);
}

complex *Mixer3D::getLeftFilter()
{
	return clFil;
}