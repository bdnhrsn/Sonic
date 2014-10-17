//
//  CustomAudioUnit.cpp
//  Demo
//
//  Created by Philadelphia Game Lab on 6/11/14.
//  Copyright (c) 2014 Philadelphia Game Lab. All rights reserved.
//

#include "CustomAudioUnit.h"

static OSStatus recordingCallback (void *inRefCon, AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData) {
    
    //Please dont remove the following commented code
    //The following code will be used if we use recording callback
    
    //TODO: Use inRefCon to access our interface object to do stuff
    //Then use inNumberFrames to figure out how much data is available and make that much space available in buffers in AudioBufferList
    
    //AudioBufferList list;
    //list.mNumberBuffers = 1;
    //list.mBuffers[0].mData = sampleBuffer;
    //list.mBuffers[0].mDataByteSize = 2* inNumberFrames;
    //list.mBuffers[0].mNumberChannels = 1;
    //AudioUnitRender([audioInterface audioUnitInstance], ioActionFlags, inTimeStamp, 1, inNumberFrames, &list);
    
    return noErr;
}

static OSStatus playbackCallback (void *inRefCon, AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData) {
    
    //Please dont remove the following commented code
    //The following code will be used for immediate debugging
    
    //Notes: ioData contains buffers (may be more than one)
    //Fill them up as much as you can. Remember to set the size value in each buffer to match how much data is in each buffer.
    
    //ioData->mBuffers[i].mNumberChannels is no. of channels per buffer
    //*ioActionFlags |= kAudioUnitRenderAction_OutputIsSilence;
    //for (UInt32 i=0; i<ioData->mNumberBuffers; ++i) {
    //    memset(ioData->mBuffers[i].mData, 10000, ioData->mBuffers[i].mDataByteSize);
    //    std::cout<<"\naksjdhka " << ioData->mBuffers[i].mDataByteSize <<" i=" << i <<" "<<inBusNumber;
    //
    //}
    
    //clock_t t1, t2;
    //t1 = clock();
    mixer3D->performMix((short *)ioData->mBuffers[0].mData, (short *) ioData->mBuffers[1].mData);
    //t2 = clock();
    //cout<<"The time consumption is "<<((double)(t2-t1))/CLOCKS_PER_SEC<<endl;

    return noErr;
}

void CustomAudioUnit::init () {
    mixer3D = new Mixer3D(BUF_SIZE, SAMPLE_RATE, BIT_DEPTH, myWorld);
    
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
    
    //Disabling IO for recording
    enableIO = 0;
    AudioUnitSetProperty(audioUnitInstance, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Input, inputBus, &enableIO, sizeof(enableIO));
    
    //Enabling IO for playback
    enableIO = 1;
    AudioUnitSetProperty(audioUnitInstance, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Output, outputBus, &enableIO, sizeof(enableIO));
    
    
    UInt32 bytesPerSample = sizeof(short) ;  //sizeof(AudioUnitSampleType);
    AudioStreamBasicDescription stereoStreamFormat = {0};
    stereoStreamFormat.mBitsPerChannel = 8 * bytesPerSample;
    stereoStreamFormat.mBytesPerFrame = bytesPerSample;
    stereoStreamFormat.mBytesPerPacket = bytesPerSample;
    stereoStreamFormat.mChannelsPerFrame = 2;   // 2 incdicates stereo
    stereoStreamFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger |
    kAudioFormatFlagsNativeEndian |
    kAudioFormatFlagIsPacked |
    kAudioFormatFlagIsNonInterleaved;
    stereoStreamFormat.mFormatID = kAudioFormatLinearPCM;
    stereoStreamFormat.mFramesPerPacket = 1;
    stereoStreamFormat.mReserved = 0;
    stereoStreamFormat.mSampleRate = SAMPLE_RATE;
    
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
    myWorld = new World;
    init();
    std::cout<<"\nConstructor";
}

CustomAudioUnit::~CustomAudioUnit() {
    AudioUnitUninitialize(audioUnitInstance);
    AudioComponentInstanceDispose(audioUnitInstance);
    delete mixer3D;
    delete myWorld;
    std::cout<<"\nDestructor";
}

void CustomAudioUnit::play() {
    //init();
    myWorld->createWriteThread();
    AudioOutputUnitStart(audioUnitInstance);
    std::cout<<"\nPlay";
}

void CustomAudioUnit::stop() {
    AudioOutputUnitStop(audioUnitInstance);
    //AudioUnitUninitialize(audioUnitInstance);
    //AudioComponentInstanceDispose(audioUnitInstance);
    std::cout<<"\nStop";
}

AudioObj* CustomAudioUnit::addAudioObjectInWorld(string wavFile, VariableForLocation x, VariableForLocation y, VariableForLocation z)
{
    if(myWorld != nullptr)
    {
        Location loc = Location(x, y, z);
        Velocity vel = Velocity();
        return myWorld->addAudioObj(loc,vel,wavFile);
    }
    else
    {
        throw("World has to be created before adding audio objects");
        return nullptr;
    }
    
}

void CustomAudioUnit::setPlayerPosition(VariableForLocation x, VariableForLocation y, VariableForLocation z)
{
    myWorld->setPlayerPosition(x, y, z);
}

void CustomAudioUnit::setPlayerBearing(float bearing)
{
    myWorld->setPlayerBearing(bearing);
}






