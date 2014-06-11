#include "stdafx.h"
#include "math.h"
#include "AudioObj.h"
#include "World.h"
#include "Player.h"
#include <iostream>
#include <vector>

int main() {
	//Here we create a world which has a player at {1,1,1}. The World class
	// has a constructor that allows us to specify where the player will
	// initially be. We can then reference the Player object using the method
	// getPlayer().
	float playerPos1[3] = {1, 1, 1};
	World world = World(playerPos1);
	Player *player = world.getPlayer();

	//We can change te player's location using the player's setLocation()
	// method. Here we change the player's position to {0,0,0}. I know it
	// seems stupid, but this is just a demonstration, okay?
	float playerPos2[3] = {0, 0, 0};
	player->setLocation(playerPos2);

	//The player's getLocation() method will return an array of three floats that
	// specify its location on the x, y, and z axes.

	//In addition to a location, the player also has a bearing. The bearing is
	// what direction the player is facing. We'll set the player's bearing to
	// 90. A bearing of 90 means the player is facing forward, or towards the
	// positive z axis. If you payed attention in precalc, you'd already know
	// that.
	float bear = 90;
	player->setBearing(bear);

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

	//Now that we have a player and audio objects, we can calculate information
	// about an object's position in relation to the player. The 
	// getOrientation(AudioObj obj) method will give us three important pieces of
	// information. It will return a float array containing the radius, zenith, and
	// azimuth values. If that made no sense to you, refer to the methods' 
	// documentation in "player.h".

	//Below is a loop that will prompt the user for an input, allowing the user to
	// move the player around. This way, we can see the numbers change.
	char input;
	std::vector<AudioObj *> activeObjList;
	do {
		//This is important. The world's updateActiveObjects() method parses through
		// all of the audio objects, and updates whether or not the object is active
		// based upon the object's relative volume to the player. This needs to be
		// done EVERY UPDATE. If you forget to call this method, an audio object that
		// used to be out of range, but is now in range, will not be processed.
		world.updateActiveObjects();

		//This prints all of the important information that we're interested in.
		std::cout << 
			"\n  Player coordinates: " << player->getLocation()[0] << ',' 
				<< player->getLocation()[1] << ',' << player->getLocation()[2] << 
			"\n      Player bearing: " << player->getBearing() << 
			"\n\nObject 1 coordinates: " << obj1->getLocation()[0] << ',' 
				<< obj1->getLocation()[0] << ',' << obj1->getLocation()[0] << 
			"\nObject 1 orientation: " << player->getOrientation(obj1)[0] << ',' 
				<< player->getOrientation(obj1)[1] << ',' << player->getOrientation(obj1)[2] << 
			"\n     Object 1 volume: " << obj1->getVolume() << 
			"\n     Relative volume: " << player->getRelativeVolume(obj1) << 
			"\n              Active: " << obj1->isActive() <<
			"\n\nObject 2 coordinates: " << obj2->getLocation()[0] << ',' 
				<<obj2->getLocation()[1] << ',' << obj2->getLocation()[2] << 
			"\nObject 2 orientation: " << player->getOrientation(obj2)[0] << ',' 
				<< player->getOrientation(obj2)[1] << ',' << player->getOrientation(obj2)[2] <<
			"\n     Object 2 volume: " << obj2->getVolume() << 
			"\n     Relative volume: " << player->getRelativeVolume(obj2) <<
			"\n              Active: " << obj2->isActive() <<
			"\n\n";

		for (int j = 0; j < world.getNumActiveObjects(); j++) {
			std::cout << world.getActiveObjects()[j]->getLocation()[2] << " ";
		}
			std::cout << "\n\n";

		//Text-based adventure style prompting.
		std::cout << "Let's move. 1 = x++; 2 = z++; 3 = x--; 4 = z--; 5 = LT; 6 = RT; 0 = Exit\n>> ";
		std::cin >> input;
		if (input == '1' || input == '2' || input == '3' || input == '4' || input == '5' || input == '6') {
			switch(input) {
				case '1':
					player->setLocation(player->getLocation()[0] + 1, player->getLocation()[1], player->getLocation()[2]);
					break;
				case '2':
					player->setLocation(player->getLocation()[0], player->getLocation()[1], player->getLocation()[2] + 1);
					break;
				case '3':
					player->setLocation(player->getLocation()[0] - 1, player->getLocation()[1], player->getLocation()[2]);
					break;
				case '4':
					player->setLocation(player->getLocation()[0], player->getLocation()[1], player->getLocation()[2] - 1);
					break;
				case '5':
					player->setBearing(fmod(player->getBearing() + 90, 360));
					break;
				case '6':
					player->setBearing(fmod(player->getBearing() + 270, 360));
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