#include "../include/velocity.h"
#include <iostream>

Velocity& Velocity::operator= (const Velocity& rhs)
{
    dx = (rhs.dx);
    dy = (rhs.dy);
    dz = (rhs.dz);
    return *this;
}

Velocity& Velocity::operator+= (const Velocity& rhs)
{
    dx = dx + rhs.dx;
    dy = dy + rhs.dy;
    dz = dz + rhs.dz;
    return *this;
}

Velocity operator+ (Velocity lhs, const Velocity& rhs)
{
    lhs += rhs;
    return lhs;
}

bool Velocity::operator< (const Velocity& rhs) const
{
    if (dx == rhs.dx){
        if (dy == rhs.dy){
            return dz < rhs.dz;
        }return dy < rhs.dy;
    }return dx < rhs.dx;
}

std::ostream& operator<< (std::ostream& os, const Velocity& obj)
{
    os << obj.getdX() <<","<< obj.getdY() <<","<< obj.getdZ();
    return os;
}

VariableForVelocity Velocity::getdX (void) const
{
    return this->dx;
}

VariableForVelocity Velocity::getdY (void) const
{
    return this->dy;
}

VariableForVelocity Velocity::getdZ (void) const
{
    return this->dz;
}
