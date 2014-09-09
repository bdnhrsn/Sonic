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

float Player::computeRadius (AudioObj* obj) const {
    return sqrt(pow(obj->getLocation().getX() - location.getX(),2)
                + pow(obj->getLocation().getY() - location.getY(),2)
                + pow(obj->getLocation().getZ() - location.getZ(),2));
}

float Player::computeZenith (AudioObj* obj) const {
    if (this->computeRadius(obj) == 0) {
		return 0;
	} else {
		return asin((obj->getLocation().getY() - location.getY()) / computeRadius(obj)) * R2D;
    }
}

float Player::computeAzimuth (AudioObj* obj) const {
    float Azimuth;
    
    float xTemp=obj->getLocation().getX() - location.getX();
    float yTemp=obj->getLocation().getY() - location.getY();
    
    if (xTemp == 0) {
        Azimuth = (yTemp>=0)?0:180;
    } else if (yTemp == 0) {
        Azimuth = (xTemp>0)?90:-90;
    } else {
        Azimuth = atan(fabs(xTemp/yTemp))*R2D;
        if(yTemp<0)Azimuth = 180-Azimuth;
        if(xTemp<0)Azimuth = -Azimuth;
    }
    
    Azimuth = Azimuth - getBearing();
    if (Azimuth > 180) {
        while (Azimuth > 180) {
            Azimuth -=360;
        }
    } else if (Azimuth <= -180) {
        while (Azimuth <= -180) {
            Azimuth += 360;
        }
    }
    
    return Azimuth;
}

float Player::computeElevation (AudioObj* obj) const {
    float elevation = 0;
    
    float xTemp=obj->getLocation().getX() - location.getX();
    float yTemp=obj->getLocation().getY() - location.getY();
    float zTemp=obj->getLocation().getZ() - location.getZ();
    
    if (zTemp == 0) {
        // object is at head level
        return 0;
    } else if (xTemp == 0 and yTemp == 0) {
        // object is directly overhead
        return 90;
    } else {
        elevation = atan(zTemp/sqrt(pow(xTemp, 2) + pow(yTemp, 2)));
        return elevation * R2D;
    }
}

float Player::computeDistanceFrom (AudioObj* obj) const{
    
    float distance;
    
    float xTemp=obj->getLocation().getX() - location.getX();
    float yTemp=obj->getLocation().getY() - location.getY();
    float zTemp=obj->getLocation().getZ() - location.getZ();
    
    distance =pow(xTemp,2)+ pow(yTemp,2) + pow(zTemp,2);
    distance = sqrt(distance);
                    
    return (distance<1) ? 1: distance;
    
}
