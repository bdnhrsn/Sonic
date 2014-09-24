# Sonic Workflow Guide

### The problem 

1. Lots of local versions of the library stored in various locations.
2. Demo iOS apps that have outdated versions of the library
3. Storing the working copy of the library within an iOS demo app.

### The solution: Github + Git submodules

- Problem 1 is easily solved with Github hosting. Anyone working on the Sonic project who plans to modify source code directly should be familiar with the basics of Git.
- Problem 2 and 3 are somewhat related. To resolve these issues, use the Sonic Library as a Git submodule within the demo app's repository. If you're lazy, you can just fork [the SonicDemo repository](https://github.com/philadelphiagamelab/SonicDemo.git) that already has this setup and use that as a starting point. If you want to import Sonic into an existing iOS app, or if you want a little more freedom in how you set things up, follow the [Setup steps](#Setup)

#### Setup 

1. Create a new github repository for your demo app. It's a good idea to initialize it with a README and a LICENSE. Assuming you've already started your XCode project, then clone the github repo to your local machine and add your project to it:

		$> git clone https://github.com/MyUsername/MyDemoApp.git
		$> cd MyDemoApp
		MyDemoApp$> mv /path/to/my/project/ .
		MyDemoApp$> git add -A
		MyDemoApp$> git commit -m "added project"
		
	Note that it might be a good idea to have your demo app live in the PGL github account. You can transfer ownership to PGL from the settings panel on the app's github page.

2. The Sonic Library now has its own Github repository independent of any demo app, so add it as a submodule to your app's repo:

		MyDemoApp $> git submodule add https://github.com/philadelphiagamelab/Sonic.git Sonic
		
3. To use the Sonic library from within your app, you'll need to add these files to your XCode project. To do this, open the project and click the (+) in the bottom right, then navigate to the Sonic directory and click "Add".

4. The iOS version of the Sonic Library uses certain Audio frameworks that aren't included by default, so you'll need to add them manually. In Xcode, click on the project file, then under "Build Phases" select "Link Binary with Libraries". Add the "AudioToolbox" and "CoreAudio" frameworks. Then your project should build. 

#### Modifying the Sonic library
        
To make changes to the Sonic library that you'd like to track with git, but that you may not want to merge into the master library yet, create your own branch of the Sonic repo from within the Sonic Library directory:
    
    	MyDemoApp$> cd Sonic
    	Sonic$> git checkout -b <yourbranchname>
    	
You can name your branch whatever you want, but it might be most convenient to have it be your first name. If you'd like other Sonic contributors to be able to view the changes you make on your branch, have the github remote track it:

		Sonic$> git push -u origin <yourbranchname>
		
Creating your own branch is similar to forking, only better. You don't have to worry about keeping your fork in sync with the upstream branch. Note that to create your own branch you'll have to be registered as a collaborator on the Sonic project or as a contributor to the Philly Game Lab Github organization.

#### Sharing your changes 

To share the changes you've made on your branch. Push them to github:

		Sonic$> git push origin <yourbranchname>
		
If your so thrilled with the changes you've made on your branch that you'd like to push those changes to the master branch (the "official" Sonic library) first merge your branch with your local master branch, then push that merge to the Github master:

		Sonic$> git checkout master
		Sonic$> git merge <yourbranchname>
		# do some merge edits if needed, commit the result
		Sonic$> git push
		
Remember that any git operations you wish to perform on the Sonic library (e.g. checking out a new branch, pull/pushing changes, etc.) need to be down from within the Sonic directory, not your app's base directory.
    	
    	 
    
        
