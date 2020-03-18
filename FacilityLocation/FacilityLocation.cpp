// FacilityLocation.cpp : Defines the entry point for the application.
//

#include "headers/FacilityLocation.h"

using namespace std;


int read_file(const char* path)
{
	string line;
	ifstream file(path);


	vector<Facility*>_facilities = vector<Facility*>();
	vector<Customer*>_customers = vector<Customer*>();


	if (file.is_open())
	{
		
		getline(file, line);
		int facilities_size = stoi(line.substr(0, line.find_first_of(" ")));
		int customers_size = stoi(line.substr(line.find_first_of(" "),line.length()));

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
	}
	return 0;
}

int main(int argv, char** argc)
{
	cout << "Hello CMake." << endl;
	read_file("C:\\Users\\ffulg\\PycharmProjects\\Operation-research\\week6\\data\\fl_3_1");
	return 0;
}



