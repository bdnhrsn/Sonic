#include "complex.h"
#include "AudioObj.h"
#include "World.h"

#ifndef MIXER3D_H
#define MIXER3D_H

class Mixer3D
{
public:
	Mixer3D(int bufSize, int smpRate, int bitD, World* w);
	void mix();
private:
	World *myWorld;
	complex *input, **output, *result, *clFil, *crFil;
	AudioObj **AOList;
	short *lFil, *rFil, *cbResult;
	int bufferSize, sampleRate, bitDepth;
	void reassignWorld(World* w);
};

#endif