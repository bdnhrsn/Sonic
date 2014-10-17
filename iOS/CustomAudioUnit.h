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

#include "../include/Mixer3D.h"

// NOTE: a BUF_SIZE of 1024 will sound bad in the simulator but good on the device.
// the opposite is true for a BUF_SIZE of 512
#define BUF_SIZE 1024
#define BIT_DEPTH 16
#define SAMPLE_RATE 44100


/**
 *@class CustomAudioUnit
 *
 * adapted from the tutorial here: http://atastypixel.com/blog/using-remoteio-audio-unit/
 */
class CustomAudioUnit {
    
    AudioUnit audioUnitInstance;
    //void *sampleBuffer;
    void init();
    //static OSStatus recordingCallback (void *inRefCon, AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData);
    //static OSStatus playbackCallback (void *inRefCon, AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData);
    World *myWorld;
    int index;

public:
    CustomAudioUnit();
    ~CustomAudioUnit();
    // TOOD: CustomAudioUnit::play(), stop() needed?
    void play();
    void stop();
    AudioObj* addAudioObjectInWorld(string, VariableForLocation, VariableForLocation, VariableForLocation);
    void setPlayerPosition(VariableForLocation, VariableForLocation, VariableForLocation);
    void setPlayerBearing(float);
};

static Mixer3D *mixer3D;

#endif /* defined(__Knobs__CustomAudioUnit__) */
