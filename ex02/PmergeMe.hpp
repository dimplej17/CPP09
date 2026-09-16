#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <cstdlib> // For std::strtol
#include <climits> // For INT_MAX
#include <ctime>
#include <iomanip>

class PmergeMe
{
	public:
	// internal struct handles the element tracking for recursion
	struct Element
	{
		int value;
		std::vector<Element> sub_elements;
	};

	struct ElementPair
	{
		Element main_elem; // larger element - main_chain
		Element pend_elem; // smaller element - pend
	};

	PmergeMe();
	PmergeMe(const PmergeMe& src);
	PmergeMe& operator=(const PmergeMe& src);
	~PmergeMe();

	void execute(int argc, char** argv);

	private:
	std::vector<Element> _parseInputToVector(int argc, char** argv);
	std::vector<Element> _fordJohnsonSort(std::vector<Element>& input_vec);
	size_t _binarySearch(const std::vector<Element>& chain, const Element& target, size_t right_bound);
		
	bool _isValidNumber(const char* str) const;
};

#endif