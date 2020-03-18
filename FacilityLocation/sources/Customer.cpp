#include "Customer.h"
#include "facility.h"

Customer::Customer(int index, double capacity, double x, double y) : _capacity(capacity), _index(index), _location(Location(x,y))
{
}

Customer::~Customer()
{
	_facility = nullptr;
}

double Customer::getCapacityRequest() 
{
	return _capacity;
}

Location& Customer::getLocation()
{
	return _location;
}