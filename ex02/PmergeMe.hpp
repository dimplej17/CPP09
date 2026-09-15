#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <cstdlib> // For std::strtol
#include <climits> // For INT_MAX

class PmergeMe
{
	public:
	// internal struct handles the element tracking for recursion
	struct Element
	{
		int value;
		std::vector<Element> sub_elements;
	};

	PmergeMe();
	PmergeMe(const PmergeMe& src);
	PmergeMe& operator=(const PmergeMe& src);
	~PmergeMe();

	void execute(int argc, char** argv);

	private:
	std::vector<int> _rawVector;
	double _vectorTime;

	std::vector<Element> _parseInputToVector(int argc, char** argv);
	std::vector<Element> _sortVector(std::vector<Element>& input_vec);
		
	bool _isValidNumber(const char* str) const;
};

#endif