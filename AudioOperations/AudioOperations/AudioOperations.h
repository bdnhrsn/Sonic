#pragma once
class AudioOperations
{
public:

	AudioOperations(FILE* in);

	~AudioOperations(void);
	
	void fadeIn(void);
private:
	int numChannels;
	int audioFormat;
	int sampleRate;
	int byteRate;
	int dataSize;
	char *original;
	int fmtSize;
	int fmtStart;
	int dataStart;
};