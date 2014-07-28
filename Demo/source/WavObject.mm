//
//  WavObject.cpp
//  Demo
//
//  Created by Philadelphia Game Lab on 7/7/14.
//  Copyright (c) 2014 Philadelphia Game Lab. All rights reserved.
//
#include "../include/WavObject.h"

/*
    void extractWavHeader(string fname)
    Parses RIFF header from given WAV file
    Parses and stores WAV format data from 'fmt' subchunk
    Parses 'data' subchunk and stores pointer to beginning of data as well as end of data
 */
void WavObject::extractWavHeader(const std::string fname) {
    
    try
    {
        bool fmt = false;
        bool data = false;
        NSString *filename = [NSString stringWithUTF8String:fname.c_str()];
        std::cout << "\nAttempting to load file: " << [filename fileSystemRepresentation] << std::endl;
        
        //NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        //NSString *docs_dir = [paths objectAtIndex:0];
        //NSString* aFile = [docs_dir stringByAppendingPathComponent: filename];
        //soundFile = fopen([aFile fileSystemRepresentation], "r");
        //std::cout<<[aFile fileSystemRepresentation]<<std::endl;
        
        //NSString *testPath = [[NSBundle mainBundle] pathForResource:@"3m40stest" ofType:@"wav"];
        NSString *testPath = [[NSBundle mainBundle] pathForResource:filename ofType:@"wav"];
        soundFile = fopen([testPath fileSystemRepresentation], "r");
        //std::cout<<[aFile fileSystemRepresentation]<<std::endl;
        
        //soundFile = fopen("/Users/philadelphiagamelab2/Deskop/Demo/Demo/InputData/input1mono.wav","r");
        //soundFile = fopen("~/test.txt","r");
        
        if (!soundFile){
            std::cout<<"\nerrno: "<<errno<<"\n";
            throw(fname);
        }
        std::cout<<"\nFile Opened.";
        // Read in the RIFF chunk into the struct
        std::cout << "Reading RIFF Header..." << std::endl;
        size_t readSize = 0;
        readSize = fread(&riff_header, sizeof(RIFF_Header), 1, soundFile);
        std::cout<<"In extractWavHeader : fread returns "<< readSize<< std::endl;
    
        std::cout<<"In extractWavHeader : RIFF Header"<<std::endl;
        for (int i=0; i<4; i++){
            std::cout<<"riff_header.chunkID["<<i << "] = " <<riff_header.chunkID[i] << std::endl;
        }
        std::cout<< "riff_header.chunkSIze : "<<riff_header.chunkSize <<std::endl;
        for (int i=0; i<4; i++){
            std::cout<<"riff_header.format["<<i << "] = " <<riff_header.format[i] << std::endl;
        }
        
        // Read in first chunk to RIFF_Header
        if ((riff_header.chunkID[0] != 'R' ||
             riff_header.chunkID[1] != 'I' ||
             riff_header.chunkID[2] != 'F' ||
             riff_header.chunkID[3] != 'F') ||
            (riff_header.format[0] != 'W' ||
             riff_header.format[1] != 'A' ||
             riff_header.format[2] != 'V' ||
             riff_header.format[3] != 'E'))
            //throw ("Invalid RIFF or WAVE Header");
        
        std::cout<<"In extractWavHeader : RIFF or WAVE Header extracted"<<std::endl;
        
        // Search for format and data subchunks
        //std::cout << "Searching for FMT and DATA in WAV file..." << std::endl;
        while(!fmt || !data) {
            // Read subchunk ID
            fread(&chunk_id, sizeof(CHUNK_ID), 1, soundFile);
            // If 'fmt' subchunk, store format data
            if (!fmt && chunk_id.chunkID[0] == 'f' &&
                chunk_id.chunkID[1] == 'm' &&
                chunk_id.chunkID[2] == 't' &&
                chunk_id.chunkID[3] == ' ') {
                fmt = true;
                fread(&wave_format, sizeof(WAVE_Format), 1, soundFile);
                // Check for extra parameters
                if (wave_format.subChunkSize > 16) {
                    int extraSize = wave_format.subChunkSize - 16;
                    fseek(soundFile, sizeof(char)*extraSize, SEEK_CUR);
                }
            } // If 'data' subchunk, store pointer to beginning and end of data
            else if(!data && chunk_id.chunkID[0] == 'd' &&
                      chunk_id.chunkID[1] == 'a' &&
                      chunk_id.chunkID[2] == 't' &&
                      chunk_id.chunkID[3] == 'a') {
                data = true;
                fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);
                startOfWavData = ftell(soundFile);
                fseek(soundFile, wave_data.subChunk2Size, SEEK_CUR);
                endOfWavData = ftell(soundFile);
            }
        }

        // Set variables passed in
        wavFileData.n = wave_data.subChunk2Size;
        //std::cout<<"Sub chunk size : "<<wave_format.subChunkSize<<std::endl;
        wavFileData.sampleRate = wave_format.sampleRate;
        std::cout<<"Sample Rate : "<<wave_format.sampleRate<<std::endl;
        wavFileData.channels = wave_format.numChannels;
        //std::cout<<"no. of channels : "<<wave_format.numChannels<<std::endl;
        wavFileData.bitDepth = wave_format.bitsPerSample;
        //std::cout<<"bit Depth : "<<wave_format.bitsPerSample<<std::endl;
        //startOfWavData = ftell(soundFile);
        
        std::cout<<"In extractWavHeader : Finished extracting"<<std::endl;
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
