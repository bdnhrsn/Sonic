#ifndef WORLD_H
#define WORLD_H

#include <stdexcept>
#include <vector>
#include <pthread.h>
#include <unistd.h>

#include "location.h"
#include "velocity.h"
#include "AudioObj.h"
#include "Player.h"

using namespace std;

// TODO: finish documenting World class
class World {

	Player player;
    vector<AudioObj *> objList;
	float threshold;
    pthread_t writeThread;
    bool isWriteThreadCreated;
    
    static void *writeAudioObjects (void *);

  public:
	static const int MAX_OBJ = 20;
    
    //Constructors
    
	/**
     Default constructor creates a player at the world's origin, {0,0,0}.
	*/
    World() : threshold(0.05), isWriteThreadCreated(false) {}

	/**
     Creates a player at the
	 location specified by the first parameter,
	 and sets the player's bearing specified by
	 the second parameter.
    */
	World(const Location& loc, const Velocity& vel, float bear) : player(Player(loc,vel, bear)), threshold(0.05), isWriteThreadCreated(false) {}

    //Getters
    
	/**
     Returns a reference to the player.
     */
	Player& getPlayer();

    /**
     Returns a reference to the audio object at the
	 specified index.
     */
	AudioObj* getAudioObj(size_t index) const;
    
	/**
     Returns the array of the player's location.
     */
    Location getPlayerLocation() const;
    
    /**
     Returns the number of audio objects in the world.
     */
    int  getNumObj();

    //Setters
    
    void setPlayerPosition(VariableForLocation x, VariableForLocation y, VariableForLocation z);
    
    void setPlayerBearing(float bearing);
    
	/**
     Adds an audio object to the world. Returns the
	 index of the created object. Sets the location
	 of the created object at the world's origin, {0,0,0}.
     */
    AudioObj* addAudioObj(const std::string);

	/**
     Adds an audio object to the world. Returns the
	 index of the created object. Sets the location
	 of the created object at the location specified
	 by the parameter.
     */
    AudioObj* addAudioObj(const Location& loc, const Velocity& vel, const std::string);
    
    /**
     Supplying the azimuth and elevation. Right now used for testing multiple audio objects
     */
    AudioObj* addAudioObj(const std::string, int Azimuth, int elevation);

    void createWriteThread(void); // TODO: Remove void arg?


};


#endif