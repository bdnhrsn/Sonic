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
    
### Demo iOS applications

[SonicDemo](https://github.com/philadelphiagamelab/SonicDemo): A starting point for your Sonic app.

[SteerAudio](https://github.com/philadelphiagamelab/SteerAudio): Use gyroscope input to shape sounds

[Knobs](https://github.com/philadelphiagamelab/knobs): Turn knobs to move an audio object around in 3D space.

### Including Sonic in your own iOS app

We are currently working on compiling Sonic into a static library. In the meantime, if you would like to import the source into your iOS project, either fork [SonicDemo](https://github.com/philadelphiagamelab/SonicDemo) or consult the [Sonic Setup Guide](SETUP.md) for manual setup instructions.

### Contributing to Sonic

If you would like to make changes to the Sonic library, first fork it. Then you can either build your own testing application around that fork or use one of our existing demo apps. If you prefer to do the latter, fork one the demo apps of your choice then change its Sonic submodule to point to your Sonic fork rather than to the master repo:

		$> git clone --recursive https://github.com/<path/to/demoapp/fork>.git
		$> cd YourFork
		YourFork$> vim .gitmodules
		# replace the Sonic url with your Sonic fork's url
		YorkFork$> git submodule sync
		
Now you can modify both the Sonic library and the application its included in and push changes to their respective forks. For more help on working with git submodules, consult the [submodule documentation](http://git-scm.com/book/en/Git-Tools-Submodules).

### More info

For more information, check out [the Sonic project webpage](http://sonic.philadelphiagamelab.org/).




    

