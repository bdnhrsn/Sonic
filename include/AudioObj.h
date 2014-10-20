#ifndef AUDIOOBJ_H
#define AUDIOOBJ_H

#include <stdexcept>
#include "location.h"
#include "velocity.h"
#include "complex.h"
#include "CircBuff.h"
#include "WavObject.h"

using namespace std;
#define BUFFER_CAPACITY 65000 // This will be capacity of circular buffer and also size of memory allocated in wavObject

class AudioObj {
	
    //friend class CircBuff< class T>;
    friend class Mixer3D;
    friend class World;
    
	Location location;
    Velocity velocity;
    
    bool active;
    float volume;
    bool repeat;
    
    CircBuff<complex> circBuff;
    WavObject wavObject;
    bool toLoadMoreData;

    void writeCircBuff (void);
    bool fillAudioData(complex *, unsigned int);

    
    public:

    // TODO: Move constructors to implementation file?
	// Creates a new audio object at the world's origin, {0,0,0}.
    AudioObj(const std::string wavFileName) : active(true), volume(1), repeat(true), circBuff(BUFFER_CAPACITY), wavObject(BUFFER_CAPACITY, wavFileName), toLoadMoreData(true) {
        wavObject.loadMoreData(32768, repeat);
        circBuff.write(wavObject.complexTempData, 32768);
        }

	// Creates a new audio object at the location specified by the parameter.
    AudioObj(const Location& loc, const Velocity& vel, const std::string wavFileName) : location(loc), velocity(vel), active(true), volume(1), repeat(true), circBuff(BUFFER_CAPACITY), wavObject(BUFFER_CAPACITY, wavFileName), toLoadMoreData(true) {
        wavObject.loadMoreData(32768, repeat);
        circBuff.write(wavObject.complexTempData, 32768);
    }
    
    ~AudioObj () { }
	
	// Returns the array of the object's location.
	Location getLocation() const;
    
    // Changes the object's location to that which is specifies in the parameter.
	void setLocation (const Location& loc);
	void setLocation (VariableForLocation x, VariableForLocation y, VariableForLocation z);
    
    Velocity getVelocity() const;

    void setVelocity (const Velocity& vel);
    void setVelocity (VariableForVelocity dx, VariableForVelocity dy, VariableForVelocity dz);
    
    // Returns the volume of the audio object.
	// This will be a value from 0 to 1.
	float getVolume() const;
    
    // Sets the volume of the audio object. This will only accept values from 0 to 1.
	void setVolume(float vol);

	// Sets the volume of the audio object to a random value in the range
	// [0.01, 1].
	void setRandomVolume();

	// Returns whether or not the object is active
	bool isActive() const;

	// Changes whether or not the object is active
	void setActive(bool active);
    
    void setRepeat(bool rep);
};

#endif
