//
//  Sonic.h
//  Demo
//
//  Created by Philadelphia Game Lab on 8/8/14.
//  Copyright (c) 2014 Philadelphia Game Lab. All rights reserved.
//

#ifndef SONIC_H
#define SONIC_H

#include "CustomAudioUnit.h"

class Sonic
{
    static CustomAudioUnit *cau;
   
    
public:
    
    
    static void createWorld();
    static AudioObj* addAudioObject(string, VariableForLocation, VariableForLocation, VariableForLocation);
    static void setPlayerLocation(VariableForLocation, VariableForLocation, VariableForLocation);
    static void setPlayerBearing(float);
    //void updatePosition();
    static void startPlaying();
    //void pausePlaying();
    static void stopPlaying();
    static void reset();
    
    ~Sonic()
    {
        delete cau;
    }
    
};


#endif
