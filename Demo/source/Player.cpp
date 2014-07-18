#include "../include/Player.h"

Location Player::getLocation() const {
    return this->location;
}

void Player::setLocation (const Location& loc) {
    this->location = loc;
}

void Player::setLocation (VariableForLocation x, VariableForLocation y, VariableForLocation z) {
    this->location = Location(x,y,z);
}

Velocity Player::getVelocity() const {
    return this->velocity;
}

void Player::setVelocity (const Velocity& vel) {
    this->velocity = vel;
}

void Player::setVelocity (VariableForVelocity dx, VariableForVelocity dy, VariableForVelocity dz) {
    this->velocity = Velocity (dx,dy,dz);
}

float Player::getBearing() const {
    return this->bearing;
}

void Player::setBearing(float bear) {
    this->bearing = bear;
}

float Player::getRadius (AudioObj* obj) const {
    return sqrt(pow(obj->getLocation().getX() - location.getX(),2)
                + pow(obj->getLocation().getY() - location.getY(),2)
                + pow(obj->getLocation().getZ() - location.getZ(),2));
}

float Player::getZenith (AudioObj* obj) const {
    if (this->getRadius(obj) == 0)
		return 0;
	else
		return (asin((obj->getLocation().getY() - location.getY()) / getRadius(obj)) * R2D);
}

float Player::getAzimuth (AudioObj* obj) const {
    float dx = obj->getLocation().getX() - location.getX();
	float dz = obj->getLocation().getZ() - location.getZ();
	float angle;
	if (dx == 0) {
		if (dz > 0)
			angle = 90;
		else if (dz < 0)
			angle = 270;
		else
			angle = 0;
	} else if (dz == 0) {
		if (dx < 0)
			angle = 180;
		else
			angle = 0;
	} else {
		angle = (float)(atan(dz/dx) * R2D);
		if (dx < 0)
			angle = 180 + angle;
		else if (dz < 0 && dx > 0)
			angle = 360 + angle;
	}
	return fmod(angle - this->bearing + 450,360);
}

//template <class T, class V>
//float *Player<T,V>::getOrientation(AudioObj<T,V> * obj) const;

float Player::getRelativeVolume (AudioObj* obj) const{
    return obj->getVolume() * (1.0 / pow(this->getRadius(obj), 2)) ;
}
