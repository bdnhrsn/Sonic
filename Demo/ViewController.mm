//
//  ViewController.m
//  Demo
//
//  Created by Philadelphia Game Lab on 6/10/14.
//  Copyright (c) 2014 Philadelphia Game Lab. All rights reserved.

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController
@synthesize audioObj1, audioObj2, audioObj3, audioObj4, audioObj5;

- (void)viewDidLoad
{
    [super viewDidLoad];

    // self.customAudioUnitInstance = new CustomAudioUnit();
    Sonic::createWorld();
    // TODO: Fails for locations (0, x, 0) for all x
    audioObj1 = Sonic::addAudioObject("3m40stest.wav", 4, 3, 1);
    // audioObj1 = Sonic::addAudioObject("input1mono.wav", 0, 0, 0);
    Sonic::setPlayerBearing(0);
    Sonic::setPlayerLocation(4, 0, 1);
    

    //audioObj2 = Sonic::addAudioObject("3m40stest.wav", 0, -1, 0);
    //audioObj3 = Sonic::addAudioObject("1minutetest.wav", 2, 0, 0);
    //audioObj4 = Sonic::addAudioObject("1minutetest.wav", 2, 0, 0);
    //audioObj5 = Sonic::addAudioObject("input1mono.wav", 0, 1, 0);
    //audioObj1->setRepeat(true);
    //audioObj1->setActive(true);
    //audioObj1->setVolume(0.1);
    //audioObj1->setLocation(1, 0, 0);
    //cout << "Azimuth = " << audioObj1->computeAzimuth() << endl;
    //myWorld.addAudioObj("1minutetest.wav", 150, 0);
    //myWorld.addAudioObj(loc1,vel,"3m40stest.wav");
    //addAudioObj("input1mono.wav", 90, 0);
    //myWorld.addAudioObj("beargrowl.wav", 30, 0);
    //myWorld.addAudioObj("catmeow.wav", 30, 0);
    //myWorld.addAudioObj("applauselight.wav", 30, 0);
    //myWorld.addAudioObj("catscreech.wav", 30, 0);
    //myWorld.addAudioObj("ghomono.wav", 90, 0);
    //myWorld.addAudioObj("zipper_1+2_mono.wav", 30, 0);
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)click:(id)sender {
    //self.customAudioUnitInstance->play();
    Sonic::startPlaying();
}

- (IBAction)click2:(id)sender {
    //self.customAudioUnitInstance->stop();
    Sonic::stopPlaying();
}

CustomAudioUnit* Sonic::cau = nullptr;
@end
