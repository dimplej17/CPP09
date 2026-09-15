
#include "PmergeMe.hpp"

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "Error: Please provide positive integers to sort." << std::endl;
		return 1;
	}

	PmergeMe program;
	program.execute(argc, argv);

	return 0;
}