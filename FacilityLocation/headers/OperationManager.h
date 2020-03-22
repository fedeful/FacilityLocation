#ifndef  OPERATION_MANAGER_H
#define OPERATION_MANAGER_H


#include <vector>
#include <functional>
#include <string>
#include <list>

#include "Facility.h"
#include "Customer.h"
#include "Location.h"

enum OPTYPE {
	MOVE = 0,
	SWITCH = 1,
	CLOSE_FACILITY = 2,
};

struct Operation {
	OPTYPE _op_type;
	int f_start;
	int f_end;
	int c_a;
	int c_b;
};

class OperationManager
{

public:
	OperationManager(std::string path, std::string input_filename, std::string output_filename, double time_limit, double obj_val);
	~OperationManager();
	double calculateSolutionCost();
	double calculateNewSolutionCostSwap(double old_cost, Customer * customer_a, Customer* costumer_b);
	double calculateNewSolutionCostFacilityChange(double old_cost, Facility* facility, Customer* cusotmer_b);
	double calculateNewSolutionCostFacilityClosure(double old_cost, Facility* facility);
	bool swapCustomer(Customer* customer_a, Customer* customer_b);
	bool facilityChange(Facility* facility, Customer* customer_b);
	std::vector<int> calculateSolution();
	bool readFile();
	bool writeFile();
	void basicAssignemnt();
	void optimize();
	void optimize_second();
	void assignAllDistance();
	bool isTabu(std::vector<int>& tabu);
	bool addTabu(std::vector<int> tabu);
	void setTabuSize(int tabu_size);

private:
	double _time_limit;
	double _obj_val;
	std::string _path;
	std::string _input_filename;
	std::string _output_filename;
	std::vector<Facility*> _facilities;
	std::vector<Customer*> _customers;
	std::list<std::vector<int>> _tabu;
	int _tabu_size;
};

#endif