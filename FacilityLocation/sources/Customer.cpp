#include <functional>
#include <numeric>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cmath>
#include "Customer.h"
#include "facility.h"

using namespace std;

struct FacilitySorter3 {
	FacilitySorter3() {};
	FacilitySorter3(Customer* customer) { this->_customer = customer; }
	bool operator() (Facility* facility_a, Facility* facility_b)
	{
		return facility_a->calculateDistance(_customer) < facility_b->calculateDistance(_customer);
	}

	Customer* _customer;
} facilitySorter3;

struct CustomerSorter3 {
	CustomerSorter3() {};
	CustomerSorter3(Customer* customer) { this->_customer = customer; }
	bool operator() (Customer* customer_a, Customer* customer_b)
	{
		return _customer->calculateDistance(customer_a) > _customer->calculateDistance(customer_a);
	}

	Customer* _customer;
} customerSorter3;

Customer::Customer(int index, double capacity, double x, double y) : _capacity(capacity), _index(index), _location(Location(x,y)), _facility(nullptr)
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

Facility* Customer::getFacility()
{
	return _facility;
}

int Customer::getIndex()
{
	return _index;
}

void Customer::setFacility(Facility* facility)
{
	_facility = facility;
}

void Customer::SetNearCustomers(std::vector<Customer*> customers)
{
	_near_customers = customers;
	sort(begin(_near_customers), end(_near_customers), CustomerSorter3(this));
}

void Customer::SetNearFacilities(std::vector<Facility*> facilities)
{
	_near_facilities = facilities;
	sort(begin(_near_facilities), end(_near_facilities), FacilitySorter3(this));
}

std::vector<Customer*> Customer::GetNearCustomers(int n)
{
	vector<Customer*> tmp = vector<Customer*>();
	for (int i = 0; i < max(int(_near_customers.size()), n); ++i)
	{
		tmp.push_back(_near_customers[i]);
	}
	return tmp;
}

std::vector<Facility*> Customer::GetNearFacilities(int n)
{
	vector<Facility*> tmp = vector<Facility*>();
	for (int i = 0; i < max(int(_near_facilities.size()), n); ++i)
	{
		tmp.push_back(_near_facilities[i]);
	}
	return tmp;
}

double Customer::calculateDistance(Customer* customer)
{
	return Location::distance(getLocation(), customer->getLocation());
}

double Customer::calculateDistance(Facility* facility)
{
	return Location::distance(getLocation(), facility->getLocation());
}

