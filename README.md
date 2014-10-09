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
    
`soundFiles/` Sound files for testing. File are arranged in subdirectories by file type. Files are named with the convention `<bit-depth>-<sample-rate>-<description>.<extension>`.

### Demo iOS applications

[SonicDemo](https://github.com/philadelphiagamelab/SonicDemo): A starting point for your Sonic app.

[SteerAudio](https://github.com/philadelphiagamelab/SteerAudio): Use gyroscope input to shape sounds

[Knobs](https://github.com/philadelphiagamelab/knobs): Turn knobs to move an audio object around in 3D space.

### Including Sonic in your own iOS app

We are currently working on compiling Sonic into a static library. In the meantime, if you would like to import the source into your iOS project, either fork [SonicDemo](https://github.com/philadelphiagamelab/SonicDemo) or consult the [Sonic Setup Guide](SETUP.md) for manual setup instructions.

### Contributing to Sonic

If you would like to make changes to the Sonic library, first fork it. Then you can either build your own testing application around that fork using [the setup instructions](SETUP.md) or use one of our existing demo apps. If you prefer to do the latter, first fork [the demo app of your choice](https://github.com/PhiladelphiaGameLab/) then change its Sonic submodule to point to your Sonic fork rather than to the master copy:
		
	$> git clone --recursive https://github.com/<path/to/demoapp/fork>.git
	$> cd YourDemoAppFork
	YourDemoAppFork$> vim .gitmodules
	# replace https://github.com/PhiladelphiaGameLab/Sonic.git with https://github.com/path/to/your/sonic/fork.git
    YourDemoAppFork$> git submodule sync
	YourDemoAppFork$> cd Sonic
	YourDemoAppFork/Sonic$> git checkout master
	YourDemoAppFork/Sonic$> git pull --rebase origin master
		
Now you can modify both the Sonic library and the application it's included in and push changes to their respective forks. Note the difference between your demo app fork--which is your editable copy of one of the Sonic demo apps--and your Sonic fork--which is your editable copy of the Sonic library itself. Also be aware that forks, unlike branching, don't stay in sync with their upstream repos (in this case the master Sonic repo) [unless you explicitly pull changes from them](http://stackoverflow.com/questions/7244321/how-to-update-github-forked-repository). 

For more help on working with git submodules, consult the [submodule documentation](http://git-scm.com/book/en/Git-Tools-Submodules).

### More info

For more information, check out [the Sonic project webpage](http://sonic.philadelphiagamelab.org/).




    

