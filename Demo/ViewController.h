//
//  ViewController.h
//  Demo
//
//  Created by Philadelphia Game Lab on 6/10/14.
//  Copyright (c) 2014 Philadelphia Game Lab. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "CustomAudioUnit.h"
#include "include/Sonic.h"

@interface ViewController : UIViewController

@property (weak, nonatomic) IBOutlet UILabel *Result;
@property int count;
@property CustomAudioUnit *customAudioUnitInstance;
- (IBAction)click:(id)sender;
- (IBAction)click2:(id)sender;

@end
