#include "../include/location.h"
#include <iostream>

Location& Location::operator= (const Location& rhs)
{
    x = (rhs.x);
    y = (rhs.y);
    z = (rhs.z);
    return *this;
}

Location& Location::operator+= (const Location& rhs)
{
    x = x + rhs.x;
    y = y + rhs.y;
    z = z + rhs.z;
    return *this;
}

Location operator+ (Location lhs, const Location& rhs)
{
    lhs += rhs;
    return lhs;
}

bool Location::operator< (const Location& rhs) const
{
    if (x == rhs.x){
        if (y == rhs.y){
            return z < rhs.z;
        }return y < rhs.y;
    }return x < rhs.x;
}

ostream& operator<< (ostream& os, Location obj)
{
    os << obj.getX() <<","<< obj.getY() <<","<< obj.getZ();
    return os;
}

VariableForLocation Location::getX (void) const
{
    return this->x;
}

VariableForLocation Location::getY (void) const
{
    return this->y;
}

VariableForLocation Location::getZ (void) const
{
    return this->z;
}
