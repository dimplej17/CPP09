
#include "PmergeMe.hpp"

PmergeMe::PmergeMe() : _rawVector() {}

PmergeMe::PmergeMe(const PmergeMe& src)
{
	this->_rawVector = src._rawVector;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& src)
{
	if (this != &src)
		this->_rawVector = src._rawVector;
	return *this;
}

PmergeMe::~PmergeMe() {}

bool PmergeMe::_isValidNumber(const char* str) const
{
	if (!str || *str == '\0')
		return false;
		
	// reject negative nos.
	if (*str == '-')
		return false;

	// Skip optional leading plus sign
	if (*str == '+')
		str++; 
	
	// Handling a string that was just "+"
	if (*str == '\0')
		return false; 
		
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return false;
		str++;
	}
	return true;
}

std::vector<PmergeMe::Element> PmergeMe::_parseInputToVector(int argc, char** argv)
{
	std::vector<Element> parsed_vector;

	for (int i = 1; i < argc; ++i)
	{
		if (!_isValidNumber(argv[i])) {
			std::cerr << "Error: Invalid argument format \"" << argv[i] << "\"" << std::endl;
			std::exit(1);
		}

		long checked_val = std::strtol(argv[i], NULL, 10);
		if (checked_val > INT_MAX)
		{
			std::cerr << "Error: Number exceeds INT_MAX limits \"" << argv[i] << "\"" << std::endl;
			std::exit(1);
		}
		int clean_int = static_cast<int>(checked_val);

		for (size_t j = 0; j < parsed_vector.size(); ++j)
		{
			if (parsed_vector[j].value == clean_int)
			{
				std::cerr << "Error: Duplicate value detected (" << clean_int << ")" << std::endl;
				std::exit(1);
			}
		}

		Element elem;
		elem.value = clean_int;
		// elem.sub_elements is empty by default when instantiated
		
		parsed_vector.push_back(elem);
	}

	if (parsed_vector.empty())
	{
		std::cerr << "Error: No numbers provided" << std::endl;
		std::exit(1);
	}

	return parsed_vector;
}

std::vector<PmergeMe::Element> PmergeMe::_sortVector(std::vector<PmergeMe::Element>& input_vec)
{

}

void PmergeMe::execute(int argc, char** argv)
{
	std::vector<PmergeMe::Element> input_vector = _parseInputToVector(argc, argv);

	// .........

	
	

	// TO-DO: LATER ALLIGATOR
	// On the first line you must display an explicit text followed by the unsorted positive
	// integer sequence.
	// On the second line you must display an explicit text followed by the sorted positive
	// integer sequence.
	// On the third line, you must display an explicit message indicating the time taken
	// by your algorithm, specifying the first container used to sort the positive integer
	// sequence.
	// On the last line you must display an explicit text indicating the time used by
	// your algorithm by specifying the second container used to sort the positive integer
	// sequence.
}
