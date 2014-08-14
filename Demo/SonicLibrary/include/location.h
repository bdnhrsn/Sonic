
#pragma once

#include <stdexcept>

typedef float VariableForLocation;
using namespace std;

class Location {
    VariableForLocation x;
    VariableForLocation y;
    VariableForLocation z;
public:
    Location () : x(0), y(0), z(0) {}
    Location (VariableForLocation x0, VariableForLocation y0, VariableForLocation z0) : x(x0), y(y0), z(z0) {}
    Location& operator= (const Location&);
	Location& operator+=(const Location&);
    bool operator< (const Location&) const;
    VariableForLocation getX (void) const;
    VariableForLocation getY (void) const;
    VariableForLocation getZ (void) const;
};
