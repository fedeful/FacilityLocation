#ifndef  CUSTOMER_H
#define CUSTOMER_H

#include "Location.h"

class Facility;

class Customer
{

public:
	Customer(int index, double capacity, double x, double y);
	~Customer();
	double getCapacityRequest();
	Location& getLocation();

private:
	int _index;
	Location _location;
	double _capacity;
	Facility* _facility;	
};

#endif