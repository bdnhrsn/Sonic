//
//  Sonic.cpp
//  Demo
//
//  Created by Philadelphia Game Lab on 8/8/14.
//  Copyright (c) 2014 Philadelphia Game Lab. All rights reserved.
//

#include "Sonic.h"

void Sonic::createWorld()
{
    Sonic::cau = new CustomAudioUnit();
}

AudioObj* Sonic::addAudioObject(string wavFileName, VariableForLocation x, VariableForLocation y, VariableForLocation z)
{
    return(Sonic::cau->addAudioObjectInWorld(wavFileName, x, y, z));
}

void Sonic::startPlaying()
{
    Sonic::cau->play();
}

void Sonic::stopPlaying()
{
    Sonic::cau->stop();
}

void Sonic::setPlayerLocation(VariableForLocation x, VariableForLocation y, VariableForLocation z)
{
    Sonic::cau->setPlayerPosition(x, y, z);
}

void Sonic::setPlayerBearing(float bearing)
{
    Sonic::cau->setPlayerBearing(bearing);
}
