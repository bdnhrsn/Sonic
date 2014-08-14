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
    static void addAudioObject(string, VariableForLocation, VariableForLocation, VariableForLocation);
    //void updatePosition();
    static void startPlaying();
    //void pausePlaying();
    static void stopPlaying();
    
    ~Sonic()
    {
        delete cau;
    }
    
};


#endif
