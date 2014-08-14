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
    float Azimuth;
    
    float xTemp=obj->getLocation().getX() - location.getX();
    float yTemp=obj->getLocation().getY() - location.getY();
    
    if(xTemp==0){
        return (yTemp>=0)?0:180;
    }if(yTemp==0){
        return (xTemp>0)?90:-90;
    }
    
    Azimuth=atan(fabs(xTemp/yTemp))*180/3.14159;
    
    if(yTemp<0)Azimuth=180-Azimuth;
    if(xTemp<0)Azimuth=-Azimuth;
    
    return Azimuth;

    
}

float Player::getDistance (AudioObj* obj) const{
    
    float distance;
    
    float xTemp=obj->getLocation().getX() - location.getX();
    float yTemp=obj->getLocation().getY() - location.getY();
    float zTemp=obj->getLocation().getZ() - location.getZ();
    
    distance =pow(xTemp,2)+ pow(yTemp,2) + pow(zTemp,2);
    distance = sqrt(distance);
                    
    return distance;
    
}
