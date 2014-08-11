//
//  Sonic.cpp
//  Demo
//
//  Created by Philadelphia Game Lab on 8/8/14.
//  Copyright (c) 2014 Philadelphia Game Lab. All rights reserved.
//

#include "../include/Sonic.h"

void Sonic::createWorld()
{
    Sonic::cau = new CustomAudioUnit();
}

void Sonic::addAudioObject(string wavFileName)
{
    Sonic::cau->addAudioObjectInWorld(wavFileName);
}

void Sonic::startPlaying()
{
    Sonic::cau->play();
}

void Sonic::stopPlaying()
{
    Sonic::cau->stop();
}

