#pragma once

#include <stdexcept>

typedef int VariableForVelocity;
using namespace std;

class Velocity {
    VariableForVelocity dx;
    VariableForVelocity dy;
    VariableForVelocity dz;
public:
    Velocity () : dx(0), dy(0), dz(0) {}
    Velocity (VariableForVelocity dx0, VariableForVelocity dy0, VariableForVelocity dz0) : dx(dx0), dy(dy0), dz(dz0) {}
    Velocity& operator= (const Velocity&);
	Velocity& operator+=(const Velocity&);
    bool operator< (const Velocity&) const;
    VariableForVelocity getdX (void) const;
    VariableForVelocity getdY (void) const;
    VariableForVelocity getdZ (void) const;
};
