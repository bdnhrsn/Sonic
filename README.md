# Sonic: A 3D Audio Library
This is Sonic, 0.1 originally built by members of [The Philadelphia Game Lab](http://philadelphiagamelab.org/).

Sonic is a library of C++ routines for performing real-time binaural audio
processing on mobile devices. The current version supports only iOS devices.

### Layout

`include/`	Header files for the source code.

`source/` 	Source code for library itself. Function definitions of functions declared in the include/ directory are included here.

`iOS/` 		Files and functions necessary to interface with iOS system calls.

`MIT\_HRTF\_Library/` Files and functions for audio processing. This code is available under the MIT license as specified [here](
    http://opensource.org/licenses/MIT).
    
### Demo applications

[SteerAudio](https://github.com/philadelphiagamelab/SteerAudio): Use gyroscope input to shape sounds

[Knobs](https://github.com/philadelphiagamelab/knobs): Turn knobs to move an audio object around in 3D space.

### More info

For more information, check out [the Sonic blog](http://sonic.philadelphiagamelab.org/).




    

