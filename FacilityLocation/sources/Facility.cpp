#include <functional>
#include <numeric>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cmath>
#include "Facility.h"
#include "Customer.h"

using namespace std;

struct FacilitySorter2 {
	FacilitySorter2() { _facility = nullptr;  };
	FacilitySorter2(Facility* facility) { this->_facility = facility; }
	bool operator() (Facility* facility_a, Facility* facility_b)
	{
		return facility_a->calculateDistance(_facility) < facility_b->calculateDistance(_facility);
	}

	Facility* _facility;
} facilitySorter2;

struct CustomerSorter2 {
	CustomerSorter2() { _facility = nullptr; }
	CustomerSorter2(Facility* facility) { this->_facility = facility; }
	bool operator() (Customer* customer_a, Customer* customer_b)
	{
		return _facility->calculateDistance(customer_a) < _facility->calculateDistance(customer_a);
	}

	Facility* _facility;
} customerSorter2;

double facilityCustomerAccumulate(double x, Customer* customer)
{
	return x + customer->getCapacityRequest();
}

Facility::Facility(int index, double setup_cost, double capacity, double x, double y) : _index(index), _setup_cost(setup_cost), _capacity(capacity), _location(Location(x,y)), _is_open(false), _all_mean_distance(0.0)
{
}

Facility::~Facility()
{ 
	for each (auto customer in _customers)
	{
		customer = nullptr;
	}

	_customers.clear();
}

double Facility::GetCapacity()
{
	return _capacity;
}

double Facility::GetSetUp()
{
	return _setup_cost;
}

Location& Facility::getLocation()
{
	return _location;
}

int Facility::getIndex()
{
	return _index;
}

double Facility::getAllMeanDistance()
{
	return _all_mean_distance;
}

void Facility::setAllMeanDistance(std::vector<Customer*>& customers) 
{
	_all_mean_distance = calculateAllDistance(customers) / customers.size();
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

vector<Customer*> Facility::getCustomers()
{
	return _customers;
}

bool Facility::removeAllCustomers()
{
	_customers.clear();
	closeFacility();
	return true;
}

bool Facility::canAddCustomer(Customer* customer) 
{	
	auto tmp_facility = customer->getFacility();
	if (tmp_facility != nullptr)
	{
		if (this == tmp_facility)
			return false;
	}
	return customer->getCapacityRequest() + calculateCustomersCapacity() <= _capacity;
}
bool Facility::canSwapCustomers(Customer* customer_a, Customer* customer_b)
{
	auto tmp_facility_a = customer_a->getFacility();
	auto tmp_facility_b = customer_b->getFacility();
	if (tmp_facility_a == nullptr || tmp_facility_b == nullptr)
		return false;
	if (tmp_facility_a == tmp_facility_b)
		return false;
	return customer_b->getCapacityRequest() - customer_a->getCapacityRequest() + calculateCustomersCapacity() <= _capacity;
}

double Facility::calculateAddCost(Customer* customer)
{
	double cost = calculateDistance(customer);
	if (_customers.size() == 0)
		cost += _setup_cost;
	return cost;
}

double Facility::calculateRemoveCost(Customer* customer)
{
	double cost = calculateDistance(customer);
	if (_customers.size() == 1)
		cost += _setup_cost;
	return -cost;
}

double Facility::calculateSwapCost(Customer* customer_a, Customer* customer_b)
{
	double cost = -calculateDistance(customer_a);
	cost += cost = calculateDistance(customer_b);
	return cost;
}

double Facility::calculateDistance(Customer* customer)
{
	return Location::distance(getLocation(), customer->getLocation());
}

double Facility::calculateDistance(Facility* facility)
{
	return Location::distance(getLocation(), facility->getLocation());
}

double Facility::calculateFacilityCost()
{
	if (isCloseFacility())
		return 0;
	else
	{
		double total_cost = calculateAllDistance(_customers);
		return total_cost + _setup_cost;
	}	
}

double Facility::calculateAllDistance(vector<Customer*>& customers)
{
	double total_cost = 0;
	for each (auto customer in customers)
		total_cost += Location::distance(getLocation(), customer->getLocation());
	return total_cost;
}

double Facility::calculateCustomersCapacity()
{
	if (_customers.size()) 
		return accumulate(_customers.begin(), _customers.end(), 0.0, facilityCustomerAccumulate);
	return 0.0;
}

bool Facility::removeCustomer(Customer* customer)
{
	auto it = find(begin(_customers), end(_customers), customer);
	if (it != _customers.end()) {
		_customers.erase(it);
		customer->setFacility(nullptr);
		if (_customers.size() == 0)
		{
			closeFacility();
		}
		return true;
	}
	return false;
}

bool Facility::addCustomer(Customer* customer)
{
	auto it = find(begin(_customers), end(_customers), customer);
	if (it == _customers.end()) {
		_customers.push_back(customer);
		customer->setFacility(this);
		if (_customers.size() == 1)
		{
			openFacility();		
		}
		return true;
	}
	return false;
}

void Facility::SetNearCustomers(std::vector<Customer*> customers)
{
	_near_customers = customers;
	sort(begin(_near_customers), end(_near_customers), CustomerSorter2(this));
}

void Facility::SetNearFacilities(std::vector<Facility*> facilities)
{
	_near_facilities = facilities;
	sort(begin(_near_facilities), end(_near_facilities), FacilitySorter2(this));
}

std::vector<Customer*> Facility::GetNearCustomers(int n)
{
	vector<Customer*> tmp = vector<Customer*>();
	for (int i = 0; i < max(int(_near_customers.size()), n); ++i)
	{
		tmp.push_back(_near_customers[i]);
	}
	return tmp;
}

std::vector<Facility*> Facility::GetNearFacilities(int n)
{
	vector<Facility*> tmp = vector<Facility*>();
	for (int i = 0; i < max(int(_near_facilities.size()), n); ++i)
	{
		tmp.push_back(_near_facilities[i]);
	}
	return tmp;
}