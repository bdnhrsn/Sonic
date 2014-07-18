//
//  WavObject.cpp
//  Demo
//
//  Created by Philadelphia Game Lab on 7/7/14.
//  Copyright (c) 2014 Philadelphia Game Lab. All rights reserved.
//
#include "../include/WavObject.h"

complex *WavObject::loadCmpWavData(const std::string fname, long *size, int *smpFreq, int *bitDepth, int *channels) {
    
    // Local var declarations
    FILE* soundFile = NULL;
    WAVE_Format wave_format;
    RIFF_Header riff_header;
    WAVE_Data wave_data;
    unsigned char* data;
    std::string openM = "rb";
    
    try
    {
        std::cout << "\nAttempting to load file: " << fname << std::endl;
        
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *docs_dir = [paths objectAtIndex:0];
        NSString* aFile = [docs_dir stringByAppendingPathComponent: @"input1mono.wav"];
        soundFile = fopen([aFile fileSystemRepresentation], "r");
            //std::cout<<[aFile fileSystemRepresentation]<<std::endl;
        
        //soundFile = fopen("/Users/philadelphiagamelab2/Deskop/Demo/Demo/InputData/input1mono.wav","r");
        //soundFile = fopen("~/test.txt","r");
        
        if (!soundFile){
            std::cout<<"\nerrno: "<<errno<<"\n";
            throw(fname);
        }
        std::cout<<"\nFile Opened.";
        // Read in the first chunk into the struct
        std::cout << "Reading RIFF Header..." << std::endl;
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
        std::cout << "Reading WAV Format..." << std::endl;
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
    
        //Read the next header.
        fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);
        while (wave_data.subChunkID[0] != 'd' ||
               wave_data.subChunkID[1] != 'a' ||
               wave_data.subChunkID[2] != 't' ||
               wave_data.subChunkID[3] != 'a' )
        {
            if( wave_data.subChunkID[0] == 'j' &&
               wave_data.subChunkID[1] == 'u' &&
               wave_data.subChunkID[2] == 'n' &&
               wave_data.subChunkID[3] == 'k' )
            {
                char junk[8];
                fread(&junk, 4, 1, soundFile);
            }
                
            fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);
        }
        //Vinay's comments: Here, we have to allocate how much ever memory we want.
        // Allocate memory for wave data
        //Vinay's comments: Here, we should use the fseek function most probablya
        std::cout << "Extracting data from file..." << std::endl;
        data = new unsigned char[wave_data.subChunk2Size];
        std::cout << "Data extracted!" << std::endl;
        
        //Vinay's comments: Here, the reading of wav file is taking place
        // Read in sound data to soundData var
        if (!fread(data, wave_data.subChunk2Size, 1, soundFile))
            throw ("error loading WAVE data into struct");
        
        
        //Fill the complex data array using the overloaded assignment operator
        std::cout << "Filling in complex array..." << std::endl;
        complex *wavData = new complex[wave_data.subChunk2Size];
        
        //Since it is 16-bit audio, we are going to convert the data into shorts, and rescale to complex floats.
        if(	wave_format.bitsPerSample == 16 )
        {
            short *shortData = (short *) data;
            for(int i = 0; i < wave_data.subChunk2Size / 2; i++)
            {
                wavData[i] = shortData[i] / (pow(2, 16) / 2.0);
            }
        }
        
        std::cout << "Complex array filled!" << std::endl << std::endl;
        
        // Set variables passed in
        *size = wave_data.subChunk2Size;
        *smpFreq = wave_format.sampleRate;
        
        // Set format data - num of channels and bits per sample
        *channels = wave_format.numChannels;
        *bitDepth = wave_format.bitsPerSample;
        
        // Clean up and return true if successful
        fclose(soundFile);
        delete[] data;
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

void WavObject::extractWavHeader(const std::string fname) {
    
    try
    {
        
        NSString *filename = [NSString stringWithCString:fname.c_str()];
        std::cout << "\nAttempting to load file: " << [filename fileSystemRepresentation] << std::endl;
        
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *docs_dir = [paths objectAtIndex:0];
        NSString* aFile = [docs_dir stringByAppendingPathComponent: filename];
        soundFile = fopen([aFile fileSystemRepresentation], "r");
        //std::cout<<[aFile fileSystemRepresentation]<<std::endl;
        
        //soundFile = fopen("/Users/philadelphiagamelab2/Deskop/Demo/Demo/InputData/input1mono.wav","r");
        //soundFile = fopen("~/test.txt","r");
        
        if (!soundFile){
            std::cout<<"\nerrno: "<<errno<<"\n";
            throw(fname);
        }
        std::cout<<"\nFile Opened.";
        // Read in the first chunk into the struct
        std::cout << "Reading RIFF Header..." << std::endl;
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
        std::cout << "Reading WAV Format..." << std::endl;
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
        
        //Read the next header.
        fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);
        while (wave_data.subChunkID[0] != 'd' ||
               wave_data.subChunkID[1] != 'a' ||
               wave_data.subChunkID[2] != 't' ||
               wave_data.subChunkID[3] != 'a' )
        {
            if( wave_data.subChunkID[0] == 'j' &&
               wave_data.subChunkID[1] == 'u' &&
               wave_data.subChunkID[2] == 'n' &&
               wave_data.subChunkID[3] == 'k' )
            {
                char junk[8];
                fread(&junk, 4, 1, soundFile);
            }
            
            fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);
        }
        
        // Set variables passed in
        wavFileData.n = wave_data.subChunk2Size;
        //std::cout<<"Sub chunk size : "<<wave_format.subChunkSize<<std::endl;
        wavFileData.sampleRate = wave_format.sampleRate;
        //std::cout<<"Sample Rate : "<<wave_format.sampleRate<<std::endl;
        wavFileData.channels = wave_format.numChannels;
        //std::cout<<"no. of channels : "<<wave_format.numChannels<<std::endl;
        wavFileData.bitDepth = wave_format.bitsPerSample;
        //std::cout<<"bit Depth : "<<wave_format.bitsPerSample<<std::endl;
        startOfWavData = ftell(soundFile);
    }
    
    catch (std::string error) {
        // Caught a bad file name
        std::cerr << error << " : trying to load "
        << fname << std::endl;
        // Clean up memory if wave loading fails
        if (soundFile != NULL)
            fclose(soundFile);
    }
}

void WavObject::loadMoreData(unsigned int size, bool repeat) {
    /*size_t sizeRead = 0;
    //int channels = wavFileData.channels;
    if (wavFileData.channels == 1) {
        if (!(sizeRead = fread(shortTempData, sizeof(short), size, soundFile))) {
            //throw ("error loading WAVE data into struct");
            fseek(soundFile, startOfWavData, SEEK_SET);
            sizeRead = fread(shortTempData, sizeof(short), size, soundFile);
        }
        
        //Since it is 16-bit audio, we are going to convert the data into shorts, and rescale to complex floats.
        if(	wave_format.bitsPerSample == 16 )
        {
            short *shortData = (short *) shortTempData;
            for(int i = 0; i < sizeRead; i++)
            {
                complexTempData[i] = shortData[i] / (pow(2, 16) / 2.0);
            }
        }
    } else if (wavFileData.channels == 2) {
        if (!(sizeRead = fread(shortTempData, sizeof(short), 2*size, soundFile))) {
            //throw ("error loading WAVE data into struct");
            fseek(soundFile, startOfWavData, SEEK_SET);
            sizeRead = fread(shortTempData, sizeof(short), 2*size, soundFile);
            
        }
        
        //Since it is 16-bit audio, we are going to convert the data into shorts, and rescale to complex floats.
        if(	wave_format.bitsPerSample == 16 )
        {
            short *shortData = (short *) shortTempData;
            for(int i = 0; i < sizeRead/2 ; i++)
            {
                complexTempData[i] = shortData[2*i] / (pow(2, 16) / 2.0);
            }
        }
    }*/
    if(repeat){
    size_t sizeRead = 0;
    int channels = wavFileData.channels;
    if (!(sizeRead = fread(shortTempData, sizeof(short), size*channels, soundFile))) {
        fseek(soundFile, startOfWavData, SEEK_SET);
        sizeRead = fread(shortTempData, sizeof(short), size*channels, soundFile);
    }
    
    if(	wave_format.bitsPerSample == 16 )
    {
        short *shortData = (short *) shortTempData;
        for(int i = 0; i < sizeRead/channels ; i++)
        {
            complexTempData[i] = shortData[i*channels] / (pow(2, 16) / 2.0);
        }
    }
    }
    else{
        size_t sizeRead = 0;
        int channels = wavFileData.channels;
        if (!(sizeRead = fread(shortTempData, sizeof(short), size*channels, soundFile))) {
            throw ("error loading WAVE data into struct");
            //fseek(soundFile, startOfWavData, SEEK_SET);
            //sizeRead = fread(shortTempData, sizeof(short), size*channels, soundFile);
        }
        
        if(	wave_format.bitsPerSample == 16 )
        {
            short *shortData = (short *) shortTempData;
            for(int i = 0; i < sizeRead/channels ; i++)
            {
                complexTempData[i] = shortData[i*channels] / (pow(2, 16) / 2.0);
            }
        }

        
    }
    
    
    
}
