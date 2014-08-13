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
@synthesize Result;
@synthesize count;

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    self.Result.text = @"Just Loaded";
    self.count = 0;
    //self.customAudioUnitInstance = new CustomAudioUnit();
    Sonic::createWorld();
    Sonic::addAudioObject("3m40stest.wav", 1, 0, 0);
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
    self.count++;
    self.Result.text = [NSString stringWithFormat:@"Clicked %d",self.count];
    //self.customAudioUnitInstance->play();
    Sonic::startPlaying();
}

- (IBAction)click2:(id)sender {
    self.count+= 2;
    self.Result.text = [NSString stringWithFormat:@"Clicked %d",self.count];
    //self.customAudioUnitInstance->stop();
    Sonic::stopPlaying();
}

CustomAudioUnit* Sonic::cau = nullptr;
@end
