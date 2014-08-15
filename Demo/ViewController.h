//
//  ViewController.h
//  Demo
//
//  Created by Philadelphia Game Lab on 6/10/14.
//  Copyright (c) 2014 Philadelphia Game Lab. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "SonicLibrary/Sonic.h"

@interface ViewController : UIViewController

@property (weak, nonatomic) IBOutlet UILabel *Result;
@property int count;
- (IBAction)click:(id)sender;
- (IBAction)click2:(id)sender;
@property AudioObj *audioObj1;
@property AudioObj *audioObj2;
@property AudioObj *audioObj3;
@property AudioObj *audioObj4;
@property AudioObj *audioObj5;

@end
