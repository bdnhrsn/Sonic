#include "stdafx.h"
#include "math.h"
#include "AudioObj.h"
#include "World.h"
#include "Listener.h"
#include <iostream>
#include <vector>

int main() {
	//Here we create a world which has a listener at {1,1,1}. The World class
	// has a constructor that allows us to specify where the listener will
	// initially be. We can then reference the Listener object using the method
	// getListener().
	float listenerPos1[3] = {1, 1, 1};
	World world = World(listenerPos1);
	Listener *listener = world.getListener();

	//We can change te listener's location using the listener's setLocation()
	// method. Here we change the listener's position to {0,0,0}. I know it
	// seems stupid, but this is just a demonstration, okay?
	float listenerPos2[3] = {0, 0, 0};
	listener->setLocation(listenerPos2);

	//The listener's getLocation() method will return an array of three floats that
	// specify its location on the x, y, and z axes.

	//In addition to a location, the listener also has a bearing. The bearing is
	// what direction the listener is facing. We'll set the listener's bearing to
	// 90. A bearing of 90 means the listener is facing forward, or towards the
	// positive z axis. If you payed attention in precalc, you'd already know
	// that.
	float bear = 90;
	listener->setBearing(bear);

	//Now we can start putting audio objects in the world. To do that, use the
	// world's addAudioObject() method. In the case of no arguments, the object
	// will be placed at {0,0,0}. But in this case, we give the method a set of
	// coordinates at which to place the object. The method returns an int, 
	// which is the index of the audio object. If you want a reference to the
	// audio object, use the world's getAudioObj(int index) method. Below, we do
	// all of this in one line of code. I hope it's not too confusing.
	float objPos[3] = {0, 0, 0};
	AudioObj *obj1 = world.getAudioObj(world.addAudioObj(objPos));
	obj1->setVolume(.5);

	float objPos2[3] = {0, 0, 5};
	AudioObj *obj2 = world.getAudioObj(world.addAudioObj(objPos2));

	//Now that we have a listener and audio objects, we can calculate information
	// about an object's position in relation to the listener. The 
	// getOrientation(AudioObj obj) method will give us three important pieces of
	// information. It will return a float array containing the radius, zenith, and
	// azimuth values. If that made no sense to you, refer to the methods' 
	// documentation in "listener.h".
	
	int nRoomCorners = 4;
	float roomX[] = {5, -5, -5, 5};
	float roomZ[] = {5, 5, -5, -5};
	Room * room1 = world.getRoom(world.addRoom(nRoomCorners, roomX, roomZ));

	//Below is a loop that will prompt the user for an input, allowing the user to
	// move the listener around. This way, we can see the numbers change.
	char input;
	std::vector<AudioObj *> activeObjList;
	do {
		//This is important. The world's updateActiveObjects() method parses through
		// all of the audio objects, and updates whether or not the object is active
		// based upon the object's relative volume to the listener. This needs to be
		// done EVERY UPDATE. If you forget to call this method, an audio object that
		// used to be out of range, but is now in range, will not be processed.
		world.updateActiveObjects();

		//This prints all of the important information that we're interested in.
		std::cout << 
			"\nListener coordinates: " << listener->getLocation()[0] << ',' 
				<< listener->getLocation()[1] << ',' << listener->getLocation()[2] << 
			"\n    Listener bearing: " << listener->getBearing() << 
			"\n\nObject 1 coordinates: " << obj1->getLocation()[0] << ',' 
				<< obj1->getLocation()[0] << ',' << obj1->getLocation()[0] << 
			"\nObject 1 orientation: " << listener->getOrientation(obj1)[0] << ',' 
				<< listener->getOrientation(obj1)[1] << ',' << listener->getOrientation(obj1)[2] << 
			"\n     Object 1 volume: " << obj1->getVolume() << 
			"\n     Relative volume: " << listener->getRelativeVolume(obj1) << 
			"\n              Active: " << obj1->isActive() <<
			"\n\nObject 2 coordinates: " << obj2->getLocation()[0] << ',' 
				<<obj2->getLocation()[1] << ',' << obj2->getLocation()[2] << 
			"\nObject 2 orientation: " << listener->getOrientation(obj2)[0] << ',' 
				<< listener->getOrientation(obj2)[1] << ',' << listener->getOrientation(obj2)[2] <<
			"\n     Object 2 volume: " << obj2->getVolume() <<
			"\n     Relative volume: " << listener->getRelativeVolume(obj2) <<
			"\n              Active: " << obj2->isActive() <<
			"\n\n       Room vertices: ";
		for (int i = 0; i < nRoomCorners; i++) {
			std::cout << "[" << room1->getVertex(i)[0] << "," << room1->getVertex(i)[1] << "] ";
		}
		std::cout <<
			"\n         Room height: " << room1->getHeight() << 
			"\n Room floor position: " << room1->getFloorPosition() <<
			"\n      Player in room: " << room1->contains(listener) <<
			"\n\n";

		//Text-based adventure style prompting.
		std::cout << "Let's move. 1 = x++; 2 = z++; 3 = x--; 4 = z--; 5 = LT; 6 = RT; 0 = Exit\n>> ";
		std::cin >> input;
		if (input == '1' || input == '2' || input == '3' || input == '4' || input == '5' || input == '6') {
			switch(input) {
				case '1':
					listener->setLocation(listener->getLocation()[0] + 1, listener->getLocation()[1], listener->getLocation()[2]);
					break;
				case '2':
					listener->setLocation(listener->getLocation()[0], listener->getLocation()[1], listener->getLocation()[2] + 1);
					break;
				case '3':
					listener->setLocation(listener->getLocation()[0] - 1, listener->getLocation()[1], listener->getLocation()[2]);
					break;
				case '4':
					listener->setLocation(listener->getLocation()[0], listener->getLocation()[1], listener->getLocation()[2] - 1);
					break;
				case '5':
					listener->setBearing(fmod(listener->getBearing() + 90, 360));
					break;
				case '6':
					listener->setBearing(fmod(listener->getBearing() + 270, 360));
					break;
			default:
				break;
			}

		} else if (input != '0') {
			//A good program generates a healthy animosity between it and its user.
			std::cout << "Bad try. Give it another shot there, genius.\n";
		}
	} while(input != '0');

	std::cout << "\n\nI hope you had fun. Really I do.";

	//This makes the program not terminate until a keystroke.
	std::cin.ignore();
	std::cin.ignore();
}