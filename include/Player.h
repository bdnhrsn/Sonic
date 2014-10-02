
#ifndef PLAYER_H
#define PLAYER_H

#include <stdexcept>
#include <math.h>

#include "AudioObj.h"
#include "location.h"

using namespace std;

// math.h works in radians; we want degrees. Multiply a radian
// value by this number (180/pi) to calculate the degree value.
#define PI 3.14159;
#define R2D 180/PI

class Player {
    
    Location location;
    Velocity velocity;
    float bearing;
    
public:
    
	// Creates a player at the world's origin, {0,0,0}, and
	// facing forward, at a bearing of 90.
    Player () : location(Location()),bearing(0) {}
    
	// Creates a player at the location specified by the
	// parameter, and facing forward, at a bearing of 90.
    Player (const Location& loc, const Velocity& vel, float bear) : location(loc), velocity(vel), bearing(bear) {}

	// Returns the array of the player's location.
	Location getLocation() const;
	
	// Changes the player's location to that which
	// is specifies in the parameter.
	void setLocation(const Location& loc);
	void setLocation(VariableForLocation x, VariableForLocation y, VariableForLocation z);

    Velocity getVelocity() const;
	
	void setVelocity(const Velocity& vel);
	void setVelocity(VariableForVelocity dx, VariableForVelocity dy, VariableForVelocity dz);

	// Returns thr player's bearing in degrees
	float getBearing() const;

	// Changes the player's bearing in degrees
	void setBearing(float bear);

	// Returns the radius between the player and the
	// object specified in the parameter. The radius is
	// the distance between the player and the object.
	float computeRadius(AudioObj* obj) const;

    // TODO: remove computeZenith??
	// Returns the zenith angle between the player and the
	// object specified in the parameter. The zenith angle is
	// the angle between the xy plane and the line between the
	// player and the object. It is the elevation angle.
	// 90 indicates the object is directly above the player.
	// -90 indicates the object is directly below the player.
	// 0 indicates the object is exactly in plane with the player.
	// DOES NOT WORK WITH HEAD TRACKING
	float computeZenith(AudioObj* obj) const;

	// Returns the azimuth angle between the player and the
	// object specified in the parameter. The azimuth angle is
	// the angle between the vector in the direction of the player's
    // bearing and the vector connecting the player to the object's
    // projection onto the xy plane. So as to be consistent with the MIT
    // HRTF library, 0 indicates the object is directly in front of the
    // player, values in the range (0, 180) indicate the object is
    // to the right of the player, and values in the range (0, -180)
    // indicate that the object is to the left of the player.
	// DOES NOT WORK WITH HEAD TRACKING
	float computeAzimuth(AudioObj* obj) const;
    
    // Returns the elevation angle between the player and the given
    // AudioObject, where the elevation angle is that between the xy plane
    // and the vector connecting the player and the object.
    float computeElevation(AudioObj* obj) const;
    
	////Returns an array containing the radius, zenith angle, and
	//// azimuth angle in the order.
	//// DOES NOT WORK WITH HEAD TRACKING
	//V *getOrientation(AudioObj<T,V> * obj) const;

	// Computes the euclidean distance of an object from the player
	float computeDistanceFrom(AudioObj* obj) const;
};

#endif