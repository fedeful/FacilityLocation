#include <functional>
#include <numeric>
#include <algorithm>
#include <vector>
#include "Facility.h"
#include "Customer.h"

using namespace std;

double facilityCustomerAccumulate(double x, Customer* customer)
{
	return x + customer->getCapacityRequest();
}

Facility::Facility(int index, double setup_cost, double capacity, double x, double y) : _index(index), _setup_cost(setup_cost), _capacity(capacity), _location(Location(x,y))
{
	_location = Location(x, y);
}

Facility::~Facility()
{ 
	for each (auto customer in _customers)
	{
		customer = nullptr;
	}

	_customers.clear();
}

Location& Facility::getLocation()
{
	return _location;
}

void Facility::closeFacility()
{
	_is_open = false;
}

void Facility::openFacility() 
{
	_is_open = true;
}

bool Facility::isCloseFacility() 
{
	return !_is_open;
}

bool Facility::canAddCustomer(Customer* customer) 
{
	return customer->getCapacityRequest() + calculateCustomersCapacity() <= _capacity;
}
bool Facility::canSwapCustomers(Customer* customer_a, Customer* customer_b)
{
	return customer_b->getCapacityRequest() - customer_a->getCapacityRequest() + calculateCustomersCapacity() <= _capacity;
}

double Facility::GetFacilityCost()
{
	double total_cost = calculateAllDistance(_customers);
	return total_cost + _setup_cost;
}

double Facility::calculateAllDistance(vector<Customer*>& customers)
{
	double total_cost = 0;
	for each (auto customer in customers)
	{
		total_cost += Location::distance(getLocation(), customer->getLocation());
	}
	return total_cost;
}

double Facility::calculateCustomersCapacity()
{
	if (_customers.size()) 
	{
		return accumulate(_customers.begin(), _customers.end(), 0.0, facilityCustomerAccumulate);
	}
	return 0.0;
}

bool Facility::removeCustomer(Customer* customer)
{
	auto it = find(begin(_customers), end(_customers), customer);
	if (it != _customers.end()) {
		_customers.erase(it);
		if (_customers.size() == 0)
		{
			closeFacility();
		}
		return true;
	}
	return false;
}

bool Facility::addCustomercustomer(Customer* customer)
{
	auto it = find(begin(_customers), end(_customers), customer);
	if (it == _customers.end()) {
		_customers.push_back(customer);
		if (_customers.size() == 1)
		{
			openFacility();
			return true;
		}
	}
	return false;
}