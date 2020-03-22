#include "headers/FacilityLocation.h"

using namespace std;

int main(int argv, char** argc)
{
	cout << fixed;
	string path = string("C:\\Users\\ffulg\\PycharmProjects\\Operation-research\\week6\\data");
	string input = string("fl_100_1");
	string output = string("test");
	OperationManager op = OperationManager(path, input, output, 0, 26000000);
	op.optimize_second();
	
	cout << "Final cost: " << op.calculateSolutionCost() << "\n";
	auto solution = op.calculateSolution();
	for (vector<int>::iterator it = solution.begin(); it != solution.end(); ++it)
		cout << " " << *it;
	return 0;
}



