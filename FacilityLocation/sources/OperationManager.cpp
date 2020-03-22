#include <chrono>
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <utility>
#include <numeric>
#include <functional>
#include <algorithm>

#include "OperationManager.h"

using namespace std;

struct FacilitySorter {
	bool operator() (Facility* facility_a,  Facility* facility_b) 
	{ 
		return (facility_a->GetCapacity() / (facility_a->GetSetUp() + facility_a->getAllMeanDistance()) > facility_b->GetCapacity() / (facility_b->GetSetUp() + facility_b->getAllMeanDistance()));
	}
} facilitySorter1;

double facilitySolutionCostAccumulate(double x, Facility* facility)
{
	return x + facility->calculateFacilityCost();
}

OperationManager::OperationManager(std::string path, std::string input_filename, std::string output_filename, double time_limit, double obj_val) : _path(path), _input_filename(input_filename), _output_filename(output_filename), _time_limit(time_limit), _obj_val(obj_val)
{
}

OperationManager::~OperationManager()
{
}

bool OperationManager::swapCustomer(Customer* customer_a, Customer* customer_b)
{
	auto facility_a = customer_a->getFacility();
	auto facility_b = customer_b->getFacility();
	facility_a->removeCustomer(customer_a);
	facility_a->addCustomer(customer_b);
	facility_b->removeCustomer(customer_b);
	facility_b->addCustomer(customer_a);
	return true;
}

bool OperationManager::facilityChange(Facility* facility, Customer* customer)
{
	auto facility_a = customer->getFacility();
	facility_a->removeCustomer(customer);
	facility->addCustomer(customer);

	return true;
}

double OperationManager::calculateSolutionCost()
{	
	return accumulate(begin(_facilities), end(_facilities), 0.0, facilitySolutionCostAccumulate);
}

double OperationManager::calculateNewSolutionCostSwap(double old_cost, Customer* customer_a, Customer* customer_b) 
{
	auto facility_a = customer_a->getFacility();
	auto facility_b = customer_b->getFacility();
	old_cost += facility_a->calculateSwapCost(customer_a, customer_b);
	old_cost += facility_b->calculateSwapCost(customer_b, customer_a);
	return old_cost;
}

double OperationManager::calculateNewSolutionCostFacilityChange(double old_cost, Facility* facility, Customer* cusotmer)
{
	auto facility_tmp = cusotmer->getFacility();
	old_cost += facility_tmp->calculateRemoveCost(cusotmer);
	old_cost += facility->calculateAddCost(cusotmer);
	return old_cost;
}

double OperationManager::calculateNewSolutionCostFacilityClosure(double old_cost, Facility* facility_a)
{
	vector<Customer*> customers;
	Facility* facility_best;
	double new_cost = 0;
	
	new_cost = old_cost - facility_a->calculateFacilityCost();
	customers = facility_a->getCustomers();
	
	
	for each (auto customer in customers)
	{
		facility_best = nullptr;
		for each (auto facility in _facilities) 
		{
			if (facility != facility_a)
				facility->canAddCustomer(customer);
		}
	}
	return 0;
}


vector<int> OperationManager::calculateSolution()
{
	vector<int> solution = vector<int>(_customers.size(),0);
	for each(auto customer in _customers)
	{
		auto facility = customer->getFacility();
		solution[customer->getIndex()] = facility->getIndex();
	}

	return solution;
}


bool OperationManager::readFile()
{
	string line;
	string in_str = _path + "\\" + _input_filename;
	ifstream file(in_str,ios::in);

	_facilities = vector<Facility*>();
	_customers = vector<Customer*>();


	if (file.is_open())
	{

		getline(file, line);
		int facilities_size = stoi(line.substr(0, line.find_first_of(" ")));
		int customers_size = stoi(line.substr(line.find_first_of(" "), line.length()));

		for (int i = 0; i < facilities_size; i++) {

			getline(file, line);

			double cost = stod(line.substr(0, line.find_first_of(" ")));
			line = line.substr(line.find_first_of(" ") + 1, line.length());
			double capacity = stod(line.substr(0, line.find_first_of(" ")));
			line = line.substr(line.find_first_of(" ") + 1, line.length());
			double x = stod(line.substr(0, line.find_first_of(" ")));
			line = line.substr(line.find_first_of(" ") + 1, line.length());
			double y = stod(line);

			Facility* tmp_facility = new Facility(i, cost, capacity, x, y);
			_facilities.push_back(tmp_facility);

		}

		for (int i = 0; i < customers_size; i++) {

			getline(file, line);

			double capacity = stod(line.substr(0, line.find_first_of(" ")));
			line = line.substr(line.find_first_of(" ") + 1, line.length());
			double x = stod(line.substr(0, line.find_first_of(" ")));
			line = line.substr(line.find_first_of(" ") + 1, line.length());
			double y = stod(line);

			Customer* tmp_Customer = new Customer(i, capacity, x, y);
			_customers.push_back(tmp_Customer);
		}

		file.close();
		return true;
	}
	else 
	{
		return false;
	}
}

bool OperationManager::writeFile()
{
	return true;
}

void OperationManager::basicAssignemnt()
{
	double distance;
	Customer* selected_customer;
	auto tmp_customers = _customers;

	for each (auto facility in _facilities) {
		facility->setAllMeanDistance(_customers);
	}
	
	sort(_facilities.begin(), _facilities.end(), facilitySorter1);


	while (true)
	{
		for each (auto facility in _facilities)
		{
			//cout << facility->getIndex() << "\n";
			selected_customer = nullptr;
			distance = -1;
			for each (auto customer in tmp_customers)
			{
				if (facility->canAddCustomer(customer))
				{
					if (distance == -1 || distance > facility->calculateDistance(customer))
					{
						selected_customer = customer;
						distance = facility->calculateDistance(customer);
					}
				}
			}

			if(selected_customer != nullptr)
			{
				facility->addCustomer(selected_customer);
				auto it = find(begin(tmp_customers), end(tmp_customers), selected_customer);
				if (it != tmp_customers.end())
					tmp_customers.erase(it);
				break;
			}
		}

		if(tmp_customers.size() == 0)
		{
			break;
		}
	}
}

void OperationManager::optimize()
{
	auto start_time = chrono::system_clock::now();
	auto time_limit = chrono::duration<double>(_time_limit);
	double tmp_best_solution = 0;
	bool found;
	double last_relax = 0;
	double relax = 0;
	_tabu = list<vector<int>>();
	setTabuSize(150);

	readFile();
	basicAssignemnt();


	Facility* tmp_facility;
	Customer* tmp_customer_1;
	Customer* tmp_cusotmer_2;

	double best_solution_value = calculateSolutionCost();
	auto solution = calculateSolution();
	cout.clear();

	while (true)
	{
		tmp_best_solution = best_solution_value;
		tmp_customer_1 = nullptr;
		tmp_cusotmer_2 = nullptr;
		tmp_facility = nullptr;
		found = false;

		auto tmp_tabu_sol = calculateSolution();

		for each(auto customer_a in _customers)
		{
			for each (auto customer_b in _customers)
			{
				if (customer_a != customer_b)
				{
					auto facility_a = customer_a->getFacility();
					auto facility_b = customer_b->getFacility();

					if (facility_a->canSwapCustomers(customer_a, customer_b)) 
					{
						double  tmp_val = calculateNewSolutionCostSwap(best_solution_value, customer_a, customer_b);

						auto a = tmp_tabu_sol[customer_a->getIndex()];
						auto b = tmp_tabu_sol[customer_b->getIndex()];
						tmp_tabu_sol[customer_a->getIndex()] = customer_b->getIndex();
						tmp_tabu_sol[customer_b->getIndex()] = customer_a->getIndex();

						if( !isTabu(tmp_tabu_sol) && tmp_val < best_solution_value + relax)
						{
							swapCustomer(customer_a, customer_b);
							best_solution_value = tmp_val;
							cout << best_solution_value << "\n";
							found = true;
						}
						else
						{
							tmp_tabu_sol[customer_a->getIndex()] = a;
							tmp_tabu_sol[customer_b->getIndex()] = b;
						}
					}				
				}
			}
		}

		for each (auto facility in _facilities)
		{
			for each (auto customer in _customers)
			{
				if (facility->canAddCustomer(customer))
				{
					auto  tmp_val = calculateNewSolutionCostFacilityChange(best_solution_value, facility, customer);
					
					auto a = tmp_tabu_sol[customer->getIndex()];
					tmp_tabu_sol[customer->getIndex()] = facility->getIndex();

					if (!isTabu(tmp_tabu_sol) && tmp_val < best_solution_value + relax)
					{
						facilityChange(facility, customer);
						best_solution_value = tmp_val;
						cout << best_solution_value << "\n";
						found = true;
					}
					else
					{
						tmp_tabu_sol[customer->getIndex()] = a;
					}
				}
			}
		}

		for each (auto customer in _customers)
		{
			if (customer->getFacility() == nullptr)
			{
				cout << "merda";
			}
		}

		//best_solution_value = tmp_best_solution;

		if (!found)
			if (best_solution_value <= _obj_val)
				break;
			else {
				
				addTabu(tmp_tabu_sol);

				if (relax == 0)
				{
					if (last_relax != 0)
						relax = last_relax;
					else
						relax = 2;
				}
				else
					relax *= relax;

			}
		else {
			last_relax = relax;
			relax = 0;
		}

		/*if (tmp_facility != nullptr)
			facilityChange(tmp_facility, tmp_customer_1);
		else
			swapCustomer(tmp_customer_1, tmp_cusotmer_2);*/

		auto end_time = chrono::system_clock::now();
		if (_time_limit > 0 && ((end_time - start_time) > time_limit))
			break;

		if (best_solution_value < _obj_val)
			break;

		cout << best_solution_value << "\n";
	}
}

void OperationManager::optimize_second()
{
	auto start_time = chrono::system_clock::now();
	auto time_limit = chrono::duration<double>(_time_limit);
	double tmp_best_solution = 0;
	bool found;
	double last_relax = 0;
	double relax = 0;
	_tabu = list<vector<int>>();
	setTabuSize(150);

	readFile();
	basicAssignemnt();
	assignAllDistance();


	Facility* tmp_facility;
	Customer* tmp_customer_1;
	Customer* tmp_cusotmer_2;
	
	int n = (int)_customers.size() / 100 * 20;
	int n2 = (int)_facilities.size() / 100 * 20;


	double best_solution_value = calculateSolutionCost();
	auto solution = calculateSolution();
	cout.clear();

	while (true)
	{
		tmp_best_solution = best_solution_value;
		tmp_customer_1 = nullptr;
		tmp_cusotmer_2 = nullptr;
		tmp_facility = nullptr;
		found = false;

		double best_tmp = 0;
		auto tmp_tabu_sol = calculateSolution();

		for each (auto customer_a in _customers)
		{
			for each (auto customer_b in customer_a->GetNearCustomers(n))
			{
				if (customer_a != customer_b)
				{
					auto facility_a = customer_a->getFacility();
					auto facility_b = customer_b->getFacility();

					if (facility_a->canSwapCustomers(customer_a, customer_b))
					{
						double  tmp_val = calculateNewSolutionCostSwap(best_solution_value, customer_a, customer_b);
						if (tmp_val < best_tmp || best_tmp == 0)
						{
							tmp_customer_1 = customer_a;
							tmp_cusotmer_2 = customer_b;
							tmp_facility = nullptr;
							best_tmp = tmp_val;
							found = true;
						}
					}
				}
			}
		}

		for each (auto facility in _facilities)
		{
			for each (auto customer in facility->GetNearCustomers(n))
			{
				if (facility->canAddCustomer(customer))
				{
					auto  tmp_val = calculateNewSolutionCostFacilityChange(best_solution_value, facility, customer);

					if (tmp_val < best_tmp || best_tmp == 0)
					{
						tmp_customer_1 = customer;
						tmp_cusotmer_2 = nullptr;
						tmp_facility = facility;
						best_tmp = tmp_val;
						found = true;
					}
				}
			}
		}

		if (tmp_facility != nullptr)
			facilityChange(tmp_facility, tmp_customer_1);
		else
			swapCustomer(tmp_customer_1, tmp_cusotmer_2);

		best_solution_value = calculateSolutionCost();

		auto end_time = chrono::system_clock::now();
		if (_time_limit > 0 && ((end_time - start_time) > time_limit))
			break;

		if (best_solution_value < _obj_val)
			break;

		cout << best_solution_value << "\n";
	}
}

void OperationManager::assignAllDistance()
{
	for each (auto customer in _customers)
	{
		customer->SetNearCustomers(_customers);
		customer->SetNearFacilities(_facilities);
	}

	for each (auto facility in _facilities)
	{
		facility->SetNearCustomers(_customers);
		facility->SetNearFacilities(_facilities);
	}
}

bool OperationManager::isTabu(std::vector<int>& tabu_cmp)
{
	for each (auto tabu in _tabu) {
		if (equal(tabu.begin(), tabu.end(), tabu_cmp.begin()))
			return true;
	}
	return false;
}

bool OperationManager::addTabu(std::vector<int> tabu)
{
	_tabu.push_front(tabu);
	if (_tabu.size() > _tabu_size)
	{
		_tabu.pop_back();
		return true;
	}
	return false;
}

void OperationManager::setTabuSize(int tabu_size)
{
	_tabu_size = tabu_size;
}