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
	Facility* getFacility();
	void SetNearCustomers(std::vector<Customer*>);
	void SetNearFacilities(std::vector<Facility*>);
	std::vector<Customer*> GetNearCustomers(int n);
	std::vector<Facility*> GetNearFacilities(int n);
	double calculateDistance(Customer* customer);
	double calculateDistance(Facility* facility);
	int getIndex();
	void setFacility(Facility* facility);

private:
	int _index;
	Location _location;
	double _capacity;
	Facility* _facility;	

	std::vector<Customer*> _near_customers;
	std::vector<Facility*> _near_facilities;
};

#endif