#ifndef  FACILITY_H
#define FACILITY_H

#include <vector>
#include "Location.h"

class Customer;

class Facility
{
public:
	
    Facility(int index, double setup_cost, double capacity, double x, double y);
	~Facility();
    void closeFacility();
    void openFacility();
    bool isCloseFacility();
    Location& getLocation();
    bool canAddCustomer(Customer* customer);
    bool canSwapCustomers(Customer* customer_a, Customer* customer_b);
    double GetFacilityCost();
    double calculateAllDistance(std::vector<Customer*>& customers);
    double calculateCustomersCapacity();
    bool removeCustomer(Customer* customer);
    bool addCustomercustomer(Customer* customer);

private:
	int _index;
    Location _location;
	double _setup_cost;
	double _capacity;
	bool _is_open;
	std::vector<Customer*> _customers;
};

#endif