//
//  CustomAudioUnit.cpp
//  Demo
//
//  Created by Philadelphia Game Lab on 6/11/14.
//  Copyright (c) 2014 Philadelphia Game Lab. All rights reserved.
//

#include "CustomAudioUnit.h"

static OSStatus recordingCallback (void *inRefCon, AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData) {
    
    //TODO: Use inRefCon to access our interface object to do stuff
    //Then use inNumberFrames to figure out how much data is available and make that much space available in buffers in AudioBufferList
    /*
    AudioBufferList list;
    
    list.mNumberBuffers = 1;
    list.mBuffers[0].mData = sampleBuffer;
    list.mBuffers[0].mDataByteSize = 2* inNumberFrames;
    list.mBuffers[0].mNumberChannels = 1;
    
    AudioUnitRender([audioInterface audioUnitInstance], ioActionFlags, inTimeStamp, 1, inNumberFrames, &list);
    */
    
    //std::cout<<"\naksjdhka " ;

    return noErr;
}





static OSStatus playbackCallback (void *inRefCon, AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData) {
    
    //Notes: ioData contains buffers (may be more than one)
    //Fill them up as much as you can. Remember to set the size value in each buffer to match how much data is in each buffer.
    
    //ioData->mBuffers[i].mNumberChannels is no. of channels per buffer
    
    //*ioActionFlags |= kAudioUnitRenderAction_OutputIsSilence;
    //for (UInt32 i=0; i<ioData->mNumberBuffers; ++i) {
    //    memset(ioData->mBuffers[i].mData, 10000, ioData->mBuffers[i].mDataByteSize);
    //    std::cout<<"\naksjdhka " << ioData->mBuffers[i].mDataByteSize <<" i=" << i <<" "<<inBusNumber;
    //
    //}
    //memset(ioData->mBuffers[0].mData, 0, 512);
    //memset(ioData->mBuffers[1].mData, 0, 512);
    //lock_t t1,t2;
    //t1=clock();
    
    mixer3D->overlapConvolution((short *)ioData->mBuffers[0].mData, (short *) ioData->mBuffers[1].mData);
    //t2=clock();
    //cout<<"The time consumption is "<<((double)(t2-t1))/CLOCKS_PER_SEC<<endl;

    //mixer3D->mix((short *)ioData->mBuffers[0].mData, (short *) ioData->mBuffers[1].mData);
    return noErr;
}

void CustomAudioUnit::init () {
<<<<<<< HEAD
    
    //addAudioObjectInWorld("3m40stest.wav");
	//myWorld.addAudioObj("1minutetest.wav", 150, 0);
=======
    Location loc = Location(1,0,0);
    Velocity vel = Velocity();
	//myWorld.addAudioObj("1minutetest.wav", 150, 0);
    myWorld.addAudioObj( loc, vel,"3m40stest.wav");
    //myWorld.addAudioObj("3m40stest.wav", -90, 0);
    //myWorld.addAudioObj("input1mono.wav", 90, 0);
    
>>>>>>> eefcbeed4b654ed6edc7138a4df9b0767ebc6fc5
    //myWorld.addAudioObj("beargrowl.wav", 30, 0);
    //myWorld.addAudioObj("catmeow.wav", 30, 0);
    //myWorld.addAudioObj("applauselight.wav", 30, 0);
    //myWorld.addAudioObj("catscreech.wav", 30, 0);
    //myWorld.addAudioObj("ghomono.wav", -30, 0);
    //myWorld.addAudioObj("zipper_1+2_mono.wav", 30, 0);
    //myWorld.getAudioObj(0)->setRandomVolume();
    
    int bufferSize = 512;
    int bitDepth = 16;
    mixer3D = new Mixer3D(bufferSize, 44100, bitDepth, &myWorld);
    
    
    AudioComponentDescription desc;
    
    desc.componentType = kAudioUnitType_Output;
    desc.componentSubType = kAudioUnitSubType_RemoteIO;
    desc.componentFlags = 0;
    desc.componentFlagsMask = 0;
    desc.componentManufacturer = kAudioUnitManufacturer_Apple;
    
    AudioComponent component = AudioComponentFindNext(NULL, &desc);
    AudioComponentInstanceNew(component, &audioUnitInstance);
    
    UInt32 enableIO;
    AudioUnitElement inputBus = 1;
    AudioUnitElement outputBus = 0;
    //sampleBuffer = malloc(4*1024);
    
    //Disabling IO for recording
    enableIO = 0;
    AudioUnitSetProperty(audioUnitInstance, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Input, inputBus, &enableIO, sizeof(enableIO));
    
    //Enabling IO for playback
    enableIO = 1;
    AudioUnitSetProperty(audioUnitInstance, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Output, outputBus, &enableIO, sizeof(enableIO));
    
    //UInt32 shouldAllocateBuffer = 1;
    //AudioUnitSetProperty(audioUnitInstance, kAudioUnitProperty_ShouldAllocateBuffer, kAudioUnitScope_Global, 1, &shouldAllocateBuffer, sizeof(shouldAllocateBuffer));
    
    //// MonoStream with each sample only 16bits
    //AudioStreamBasicDescription monoStreamFormat;
    //monoStreamFormat.mBitsPerChannel = 16;
    //monoStreamFormat.mBytesPerFrame = 2;
    //monoStreamFormat.mBytesPerPacket = 2;
    //monoStreamFormat.mChannelsPerFrame = 1;
    //monoStreamFormat.mFormatFlags = kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;
    //monoStreamFormat.mFormatID = kAudioFormatLinearPCM;
    //monoStreamFormat.mFramesPerPacket = 1;
    //monoStreamFormat.mReserved = 0;
    //monoStreamFormat.mSampleRate = 44100.0;
    
    size_t bytesPerSample = sizeof(short) ;  //sizeof(AudioUnitSampleType);
    AudioStreamBasicDescription stereoStreamFormat = {0};
    stereoStreamFormat.mBitsPerChannel = 8 * bytesPerSample;
    stereoStreamFormat.mBytesPerFrame = bytesPerSample;
    stereoStreamFormat.mBytesPerPacket = bytesPerSample;
    stereoStreamFormat.mChannelsPerFrame = 2;   // 2 incdicates stereo
    // kAudioFormatFlagsCanonical | kAudioFormatFlagIsNonInterleaved = kAudioFormatFlagsAudioUnitCanonical
    // If interleaved, mBytesPerFrame = no.ofChannels * bytesPerSample
    // If interleaved, there is only one buffer in AudioBufferList
        /*kAudioFormatFlagIsSignedInteger |
         kAudioFormatFlagsNativeEndian |
         kAudioFormatFlagIsPacked |
         kAudioFormatFlagIsNonInterleaved |
         (kAudioUnitSampleFractionBits <<
         kLinearPCMFormatFlagsSampleFractionShift)*/
    stereoStreamFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger |
    kAudioFormatFlagsNativeEndian |
    kAudioFormatFlagIsPacked |
    kAudioFormatFlagIsNonInterleaved;
    stereoStreamFormat.mFormatID = kAudioFormatLinearPCM;
    stereoStreamFormat.mFramesPerPacket = 1;
    stereoStreamFormat.mReserved = 0;
    stereoStreamFormat.mSampleRate = 44100.0;
    
    AudioUnitSetProperty(audioUnitInstance, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, inputBus, &stereoStreamFormat, sizeof(AudioStreamBasicDescription));
    AudioUnitSetProperty(audioUnitInstance, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, outputBus, &stereoStreamFormat, sizeof(AudioStreamBasicDescription));
    
    //Setting input callback
    AURenderCallbackStruct callbackStruct;
    callbackStruct.inputProc = &recordingCallback;    //////Should there be an ampersand
    callbackStruct.inputProcRefCon = audioUnitInstance;
    AudioUnitSetProperty(audioUnitInstance, kAudioOutputUnitProperty_SetInputCallback, kAudioUnitScope_Output, inputBus, &callbackStruct, sizeof(callbackStruct));   /////Not sure of scope and bus/element
 
    //Setting output callback
    callbackStruct.inputProc = &playbackCallback;
    callbackStruct.inputProcRefCon = audioUnitInstance;
    AudioUnitSetProperty(audioUnitInstance, kAudioUnitProperty_SetRenderCallback, kAudioUnitScope_Input, outputBus, &callbackStruct, sizeof(callbackStruct));
    
    AudioUnitInitialize(audioUnitInstance);

}
 
CustomAudioUnit::CustomAudioUnit() {
    init();
    std::cout<<"\nConstructor";
}

CustomAudioUnit::~CustomAudioUnit() {
    AudioUnitUninitialize(audioUnitInstance);
    AudioComponentInstanceDispose(audioUnitInstance);
    std::cout<<"\nDestructor";
}

void CustomAudioUnit::play() {
    //init();
    myWorld.createWriteThread();
    AudioOutputUnitStart(audioUnitInstance);
    std::cout<<"\nPlay";
}

void CustomAudioUnit::stop() {
    AudioOutputUnitStop(audioUnitInstance);
    //AudioUnitUninitialize(audioUnitInstance);
    //AudioComponentInstanceDispose(audioUnitInstance);
    std::cout<<"\nStop";
}

void CustomAudioUnit::addAudioObjectInWorld(string wavFile)
{
    myWorld.addAudioObj(wavFile, -90, 0);
    //myWorld.addAudioObj("input1mono.wav", 90, 0);
    
}



