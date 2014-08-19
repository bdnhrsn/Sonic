
#ifndef PLAYER_H
#define PLAYER_H

#include <stdexcept>
#include <math.h>

#include "AudioObj.h"
#include "location.h"

using namespace std;

//math.h works in radians; we want degrees. Multiply a radian
// value by this number (180/pi) to calculate the degree value.
#define R2D 57.2957795131

class Player {
    
    Location location;
    Velocity velocity;
    float bearing;
    
public:
    
	//Creates a player at the world's origin, {0,0,0}, and
	// facing forward, at a bearing of 90.
    Player () : location(Location()),bearing(0) {}
    
	//Creates a player at the location specified by the
	// parameter, and facing forward, at a bearing of 90.
    Player (const Location& loc, const Velocity& vel, float bear) : location(loc), velocity(vel), bearing(bear) {}

	//Returns the array of the player's location.
	Location getLocation() const;
	
	//Changes the player's location to that which
	// is specifies in the parameter.
	void setLocation(const Location& loc);
	void setLocation(VariableForLocation x, VariableForLocation y, VariableForLocation z);

    Velocity getVelocity() const;
	
	void setVelocity(const Velocity& vel);
	void setVelocity(VariableForVelocity dx, VariableForVelocity dy, VariableForVelocity dz);

	//Returns thr player's bearing in degrees
	float getBearing() const;

	//Changes the player's bearing in degrees
	void setBearing(float bear);

	//Returns the radius between the player and the
	// object specified in the parameter. The radius is
	// the distance between the player and the object.
	float getRadius(AudioObj* obj) const;

	//Returns the zenith angle between the player and the
	// object specified in the parameter. The zenith angle is
	// the angle between the xz plane and the line between the
	// player and the object. It is the elevation angle.
	// 90 indicates the object is directly above the player.
	// -90 indicates the object is directly below the player.
	// 0 indicates the object is exactly in plane with the player.
	// DOES NOT WORK WITH HEAD TRACKING
	float getZenith(AudioObj* obj) const;

	//Returns the azimuth angle between the player and the
	// object specified in the parameter. The azimuth angle is
	// the angle between the positive x axis and the line between 
	// the player and the object's projection onto the xz plane.
	// 0 indicates the object is directly to the player's right.
	// 90 indicates the object is directly in front of the player.
	// 180 indicates the object is directly to the player's left.
	// 270 indicates the object is directly behind the player.
	// DOES NOT WORK WITH HEAD TRACKING
	float getAzimuth(AudioObj* obj) const;

	////Returns an array containing the radius, zenith angle, and
	//// azimuth angle in the order.
	//// DOES NOT WORK WITH HEAD TRACKING
	//V *getOrientation(AudioObj<T,V> * obj) const;

	//Achieve the distance difference between the targetted audioObject and player
	float getDistance(AudioObj* obj) const;
};

#endif