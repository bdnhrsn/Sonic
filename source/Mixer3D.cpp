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
	lFil = new short[MIT_HRTF_44_TAPS];
	rFil = new short[MIT_HRTF_44_TAPS];
	clFil = new complex[MIT_HRTF_44_TAPS];
	crFil = new complex[MIT_HRTF_44_TAPS];
}

void Mixer3D::mix()
{
	reset();
	AOList = myWorld->getActiveObjects();
	int nActive = myWorld->getNumActiveObjects();
	for (int i = 0; i < nActive; i++)
		process(AOList[i], i);
}