# Sonic Setup Instructions

Use this guide to import [Sonic](https://github.com/philadelphiagamelab/Sonic), the open-source 3D-Audio library from [The Philadelphia Game Lab](www.philadelphiagamelab.org), into your application. Currently Sonic only supports iOS though compatability with other platforms, namely Android, is in the works.

#### iOS Setup

If you're just starting your application, consider using the [SonicDemo project](https://github.com/ as a starting point. If for whatever reason you'd rather set things up manually, follow these instructions:

1. Create a new github repository for your demo app. It's a good idea to initialize it with a README and a LICENSE. Then clone the github repo to your local machine and, assuming you've already started your XCode project, add your project to it:

		$> git clone https://github.com/MyUsername/MyDemoApp.git
		$> cd MyDemoApp
		MyDemoApp$> mv /path/to/my/project/ .
		MyDemoApp$> git add -A
		MyDemoApp$> git commit -m "added project"
		
	Note that if your a PGL employee might be a good idea to have your demo app live in the PGL github account. You can transfer ownership to PGL from the settings panel on the app's github page.
	
	Also note that though Github is very useful and it is highly recommended that you use it, it's not required to create a git repository for your application. If you choose not do so, replace `git submodule add` in the following step with `git clone`.
	
2. The Sonic Library now has its own Github repository independent of any demo app, so add it as a submodule to your app's repo:

		MyDemoApp $> git submodule add https://github.com/philadelphiagamelab/Sonic.git 
3. To use the Sonic library from within your app, you'll need to add these files to your XCode project. To do this, open the project and click the (+) in the bottom right, then navigate to the Sonic directory and click "Add".

4. The iOS version of the Sonic Library uses certain Audio frameworks that aren't included by default, so you'll need to add them manually. In Xcode, click on the project file, then under "Build Phases" select "Link Binary with Libraries". Add the "AudioToolbox" and "CoreAudio" frameworks.

5. In your `ViewController.h` file, be sure to import the Sonic library via `#import "Sonic.h`. Also include the following line of code in your `ViewController.m` file, which you should rename to `ViewController.mm` ([here's why](http://stackoverflow.com/a/4562280/1402368)):

		CustomAudioUnit* Sonic::cau = nullptr;

	Note that if you have multiple ViewControllers you will need to change the extension on all of their implementations from `.m` to `.mm`. If you don't do these things and try to build, you will get library linking errors.
	
#### Android Setup

Coming soon...
