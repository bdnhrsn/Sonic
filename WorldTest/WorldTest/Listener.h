#ifndef LISTENER_H
#define LISTENER_H

//math.h works in radians; we want degrees. Multiply a radian 
// value by this number (180/pi) to calculate the degree value.
#define R2D 57.2957795131

#include "AudioObj.h"

class Listener {

  private:

	float location [3];
	float bearing;

  public:

	//Creates a listener at the world's origin, {0,0,0}, and
	// facing forward, at a bearing of 90.
	Listener::Listener();

	//Creates a listener at the location specified by the
	// parameter, and facing forward, at a bearing of 90.
	Listener::Listener(float loc[]);

	//Creates a listener at the location specified by the
	// first parameter, with a bearing specified by the
	// second parameter.
	Listener::Listener(float loc[], float bear);

	//Returns the array of the listener's location.
	float* getLocation();
	
	//Changes the listener's location to that which
	// is specifies in the parameter.
	void setLocation(float loc[]);
	void setLocation(float x, float y, float z);

	//Returns thr listener's bearing in degrees
	float getBearing();

	//Changes the listener's bearing in degrees
	void setBearing(float bear);

	//Returns the radius between the listener and the
	// object specified in the parameter. The radius is
	// the distance between the listener and the object.
	float getRadius(AudioObj * obj);

	//Returns the zenith angle between the listener and the
	// object specified in the parameter. The zenith angle is
	// the angle between the xz plane and the line between the
	// listener and the object. It is the elevation angle.
	// 90 indicates the object is directly above the listener.
	// -90 indicates the object is directly below the listener.
	// 0 indicates the object is exactly in plane with the listener.
	// DOES NOT WORK WITH HEAD TRACKING
	float getZenith(AudioObj * obj);

	//Returns the azimuth angle between the listener and the
	// object specified in the parameter. The azimuth angle is
	// the angle between the positive x axis and the line between 
	// the listener and the object's projection onto the xz plane.
	// 0 indicates the object is directly to the listener's right.
	// 90 indicates the object is directly in front of the listener.
	// 180 indicates the object is directly to the listener's left.
	// 270 indicates the object is directly behind the listener.
	// DOES NOT WORK WITH HEAD TRACKING
	float getAzimuth(AudioObj * obj);
	
	//Returns the azimuth angle with respect to the listener's
	// left ear.
	// DOES NOT WORK WITH HEAD TRACKING
	float getLeftAzimuth(AudioObj * obj);
	
	//Returns the azimuth angle with respect to the listener's
	// right ear.
	// DOES NOT WORK WITH HEAD TRACKING
	float getRightAzimuth(AudioObj * obj);

	//Returns an array containing the radius, zenith angle, and
	// azimuth angle in the order.
	// DOES NOT WORK WITH HEAD TRACKING
	float * getOrientation(AudioObj * obj);

	//Returns a float between 0 and 1 that describes how loud
	// the object is in relation to the player. This is 
	// calculated using the inverse square law. Math.
	float getRelativeVolume(AudioObj * obj);
};

#endif