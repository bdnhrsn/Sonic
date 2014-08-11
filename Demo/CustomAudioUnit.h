//
//  CustomAudioUnit.h
//  Demo
//
//  Created by Philadelphia Game Lab on 6/11/14.
//  Copyright (c) 2014 Philadelphia Game Lab. All rights reserved.
//

#ifndef __Demo__CustomAudioUnit__
#define __Demo__CustomAudioUnit__

//#include <CoreAudio/CoreAudioTypes.h>
//#include <AudioUnit/AudioUnit.h>
#include <AudioToolbox/AudioToolbox.h>
#include <iostream>

#include "include/Mixer3D.h"

class CustomAudioUnit {
    
    AudioUnit audioUnitInstance;
    //void *sampleBuffer;
    void init();
    //static OSStatus recordingCallback (void *inRefCon, AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData);
    //static OSStatus playbackCallback (void *inRefCon, AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData);
    World myWorld;
public:
    CustomAudioUnit();
    ~CustomAudioUnit();
    void play();
    void stop();
    void addAudioObjectInWorld(string);
    
    
};
static Mixer3D *mixer3D;


#endif /* defined(__Demo__CustomAudioUnit__) */
