#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "AudioOperations.h"

AudioOperations::AudioOperations(FILE* in) {
	fseek(in, 0, SEEK_END);
	long fsize = ftell(in);
	fseek(in, 0, SEEK_SET);

	original = (char *)malloc(fsize + 1);
	fread(original, fsize, 1, in);

	original[fsize] = 0;

	int i = 12;

	while (!(original[i] == 'f' && original[i + 1] == 'm' && original[i + 2] == 't')) {
		i++;
	}

	fmtStart = i;

	fseek(in, fmtStart+4, 0);
	fread(&fmtSize, 4, 1, in);

	audioFormat = original[i+8];

	numChannels = original[i+10];

	fseek(in, fmtStart+12, 0);
	fread(&sampleRate, 4, 1, in);

	fseek(in, fmtStart+16, 0);
	fread(&byteRate, 4, 1, in);
	i = 12;

	while (!(original[i] == 'd' && original[i + 1] == 'a' && original[i + 2] == 't' && original[i + 3] == 'a')) {
		i++;
	}

	dataStart = i;

	fseek(in, dataStart+4, 0);
	fread(&dataSize, 4, 1, in);

	fclose(in);
}

AudioOperations::~AudioOperations(void) {

}

void AudioOperations::fadeIn(void) {
	std::ofstream out("out.wav", std::ios::binary);

	for (int i = 0; i < 12; i++) {
		out << original[i];
	}

	for (int i = 0; i < fmtSize+8; i++) {
		out << original[fmtStart + i];
	}

	for (int i = 0; i < 8; i++) {
		out << original[dataStart+i];
	}

	int n = 0;
	//char next [4];
	n = (((int)*(original+dataStart+10)*256 + (int)*(original+dataStart+10+1)));
	//_itoa(n,next,16);
	//if (n < 16) {
	//	std::cout << 0;
	//} else if (n < 256) {
	//	std::cout << 0;
	//} else if (n < 4096) {
	//	std::cout << 0;
	//}
	//
	//std::cout << (char)(n/256);

	double multiplier;

	for (int i = 0; i < dataSize; i++) {
		if (i < 1000000) {
			multiplier = (double)i / 1000000.0;
			n = (int)((int)*(original+dataStart+8+i)*256 + (int)*(original+dataStart+8+i+1))*multiplier;
			//std::cout << n << ',';
			out << (char)(n/256);
			out << (char)(n%256);
			i++;
		} else {
			out << original[dataStart+8+i];
		}
	}

	std::cout << "Fade In Complete";
}