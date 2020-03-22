#ifndef  FACILITY_H
#define FACILITY_H

#include <vector>
#include "Location.h"

class Customer;

class Facility
{
public:
	
    // CONSTRUCTOR //
    Facility(int index, double setup_cost, double capacity, double x, double y);
	~Facility();
    // UTILITY //
    bool isCloseFacility();
    // GETTER //
    std::vector<Customer*> GetNearCustomers(int n);
    std::vector<Facility*> GetNearFacilities(int n);
    Location& getLocation();
    std::vector<Customer*> getCustomers();
    double GetCapacity();
    double GetSetUp();
    int getIndex();
    double getAllMeanDistance();
    // SETTER //
    void setAllMeanDistance(std::vector<Customer*>& customers);
    void SetNearCustomers(std::vector<Customer*>);
    void SetNearFacilities(std::vector<Facility*>);
    // CHECK OPERATION //
    bool canAddCustomer(Customer* customer);
    bool canSwapCustomers(Customer* customer_a, Customer* customer_b);
    // COST CALCULATION //
    double calculateAddCost(Customer* customer);
    double calculateRemoveCost(Customer* customer);
    double calculateSwapCost(Customer* customer_a, Customer* customer_b);
    double calculateDistance(Customer* customer);
    double calculateDistance(Facility* facility);
    double calculateFacilityCost();
    double calculateAllDistance(std::vector<Customer*>& customers);
    double calculateCustomersCapacity();
    // MOVING OPERATION //
    bool removeAllCustomers();
    bool removeCustomer(Customer* customer);
    bool addCustomer(Customer* customer);

private:
	int _index;
    Location _location;
	double _setup_cost;
	double _capacity;
	bool _is_open;
	std::vector<Customer*> _customers;
    double _all_mean_distance;

    std::vector<Customer*> _near_customers;
    std::vector<Facility*> _near_facilities;

    void closeFacility();
    void openFacility();
};

#endif