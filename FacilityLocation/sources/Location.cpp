#include "Location.h"
#include <cmath>

using namespace std;

// constructor

Location::Location(double x, double y) : _x(x), _y(y) {}

Location::~Location(){}

// getter - setter

double Location::getX() { return _x; }

double Location::getY() { return _y; }

// function

double Location::locationsDistance(Location& location) { return Location::distance(*this, location); }

double Location::distance(Location& l_location, Location& r_location) {
	return sqrt(pow((l_location.getX() - r_location.getX()), 2) + pow((l_location.getY() - r_location.getY()), 2));
}