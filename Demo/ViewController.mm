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

    Sonic::createWorld();
    // 45 degree elevation
    // audioObj1 = Sonic::addAudioObject("input1mono.wav", -1*.7071, 0, .7071);
    // 60 degree elevation
    // audioObj1 = Sonic::addAudioObject("input1mono.wav", -1*.866, 0, .866);
    audioObj2 = Sonic::addAudioObject("3m40stest.wav", 0, 0, 0);
    
    Sonic::setPlayerBearing(0);
    Sonic::setPlayerLocation(0, 0, 0);
    Sonic::startPlaying();
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)click:(id)sender {
    //self.customAudioUnitInstance->play();
    // Sonic::startPlaying();
}

- (IBAction)click2:(id)sender {
    //self.customAudioUnitInstance->stop();
    Sonic::stopPlaying();
}

CustomAudioUnit* Sonic::cau = nullptr;
@end
