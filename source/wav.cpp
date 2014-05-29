#include "../include/wav.h"
#include "../include/complex.h"

#include <iostream>

namespace utility 
{
	/*
		Struct that holds RIFF data of Wave File.
		RIFF data is meta data info that holds ID, size,
		and format of WAVE file.
	*/
	struct RIFF_Header {
		char chunkID[4];
		long chuckSize;
		char format[4];
	};

	/*
	Struct that holds format subchunk data for WAVE file.
	*/
	struct WAVE_Format {
		char subChunkID[4];
		long subChunkSize;
		short audioFormat;
		short numChannels;
		long sampleRate;
		long byteRate;
		short blockAlign;
		short bitsPerSample;
	};

	/*
	Struct that holds data of WAVE file.
	*/
	struct WAVE_Data {
		char subChunkID[4];
		long subChunk2Size;
	};

	complex *loadCmpWavData(const std::string fname, long *size, int *smpFreq, int *bitDepth, int *channels)
	{
		// Local var declarations
		FILE* soundFile = NULL;
		WAVE_Format wave_format;
		RIFF_Header riff_header;
		WAVE_Data wave_data;
		unsigned char* data;

		try 
		{
			std::cout << "Attempting to load file: " << fname << std::endl;
			soundFile = fopen(fname.c_str(), "rb");

			if (!soundFile)
				throw(fname);

			// Read in the first chunk into the struct
			fread(&riff_header, sizeof(RIFF_Header), 1, soundFile);

			// Read in first chunk to RIFF_Header
			if ((riff_header.chunkID[0] != 'R' ||
				riff_header.chunkID[1] != 'I' ||
				riff_header.chunkID[2] != 'F' ||
				riff_header.chunkID[3] != 'F') ||
				(riff_header.format[0] != 'W' ||
				riff_header.format[1] != 'A' ||
				riff_header.format[2] != 'V' ||
				riff_header.format[3] != 'E'))
				throw ("Invalid RIFF or WAVE Header");

			// Read in second chunk to WAVE_Format
			fread(&wave_format, sizeof(WAVE_Format), 1, soundFile);
			// Check for format tag in memory
			if (wave_format.subChunkID[0] != 'f' ||
				wave_format.subChunkID[1] != 'm' ||
				wave_format.subChunkID[2] != 't' ||
				wave_format.subChunkID[3] != ' ')
				throw ("Invalid Wave Format");

			// Check for extra parameters
			if (wave_format.subChunkSize > 16) {
				int extraSize = wave_format.subChunkSize - 16;
				fseek(soundFile, sizeof(char)*extraSize, SEEK_CUR);
			}

			// Read last byte of data before sound data
			fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);
			// Check for data tag in memory
			if (wave_data.subChunkID[0] != 'd' ||
				wave_data.subChunkID[1] != 'a' ||
				wave_data.subChunkID[2] != 't' ||
				wave_data.subChunkID[3] != 'a')
				throw ("Invalid data header");
			
			// Allocate memory for wave data
			std::cout << "Extracting data from file..." << std::endl;
			data = new unsigned char[wave_data.subChunk2Size];
			std::cout << "Data extracted!" << std::endl;
			
			// Read in sound data to soundData var
			if (!fread(data, wave_data.subChunk2Size, 1, soundFile))
				throw ("error loading WAVE data into struct");

			//Fill the complex data array using the overloaded assignment operator
			std::cout << "Filling in complex array..." << std::endl;
			complex *wavData = new complex[wave_data.subChunk2Size];
			for(int i = 0; i < wave_data.subChunk2Size; i++)
				wavData[i] = data[i];

			std::cout << "Complex array filled!" << std::endl;

			// Set variables passed in
			*size = wave_data.subChunk2Size;
			*smpFreq = wave_format.sampleRate;

			// Set format data - num of channels and bits per sample
			*channels = wave_format.numChannels;
			*bitDepth = wave_format.bitsPerSample;

			// Clean up and return true if successful
			fclose(soundFile);
			return wavData;
		}

		catch (std::string error) {
			// Caught a bad file name
			std::cerr << error << " : trying to load "
				<< fname << std::endl;
			// Clean up memory if wave loading fails
			if (soundFile != NULL)
				fclose(soundFile);
			return NULL;
		}

		return NULL;
	}
}
