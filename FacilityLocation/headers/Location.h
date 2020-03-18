#ifndef  LOCATION_H
#define LOCATION_H

class Location
{
public:
	Location(double x, double y);
	~Location();
	double locationsDistance(Location& location);
	static double distance(Location& l_location, Location& r_location);
	double getX();
	double getY();

private:
	double _x;
	double _y;
};

#endif