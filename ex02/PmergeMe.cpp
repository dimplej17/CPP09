
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

size_t PmergeMe::_binarySearch(const std::vector<Element>& chain, const Element& target, size_t right_bound)
{
	size_t left = 0;
	size_t right = right_bound; // Constrained upper limit

	while (left < right)
	{
		size_t mid = left + (right - left) / 2;

		if (chain[mid].value < target.value)
			left = mid + 1; // Target belongs in the right half
		else
			right = mid; // Target belongs in the left half
	}
	return left; // exact insertion index
}


std::vector<PmergeMe::Element> PmergeMe::_fordJohnsonSort(std::vector<PmergeMe::Element>& input_vec)
{
	// BASE CASE: If the vector has 0 or 1 element, it's already sorted
	if (input_vec.size() <= 1)
		return input_vec;

	std::vector<ElementPair> pairs;
	Element leftover;
	bool has_leftover = false; // check for even/odd in input_vec 

	// Group input_vec into pairs
	for (size_t i = 0; i < input_vec.size(); i += 2) {
		if (i + 1 < input_vec.size())
		{
			ElementPair p;
			// Compare and assign larger to main_elem, smaller to pend_elem
			if (input_vec[i].value > input_vec[i + 1].value)
			{
				p.main_elem = input_vec[i];
				p.pend_elem = input_vec[i + 1];
			} 
			else
			{
				p.main_elem = input_vec[i + 1];
				p.pend_elem = input_vec[i];
			}
			pairs.push_back(p);
		}
		else
		{
			leftover = input_vec[i];
			has_leftover = true;
		}
	}

	std::vector<Element> next_level_input;
	// Attach pend_elem to main_elem's sub_elements vector (so that the paired partners can be tracked with main_elem)
	for (size_t i = 0; i < pairs.size(); ++i)
	{
		pairs[i].main_elem.sub_elements.push_back(pairs[i].pend_elem);
		next_level_input.push_back(pairs[i].main_elem);
	}

	// Recursive call
	std::vector<Element> sorted_main_chain = _fordJohnsonSort(next_level_input);

	// Insertion
	std::vector<Element> pend;

	// get partner from each element in sorted main_chain
	for (size_t i = 0; i < sorted_main_chain.size(); ++i)
	{
		pend.push_back(sorted_main_chain[i].sub_elements.back()); // partner is the last element inside the sub_elements vector
		sorted_main_chain[i].sub_elements.pop_back(); // remove it when done
	}

	sorted_main_chain.insert(sorted_main_chain.begin(), pend[0]);

	size_t jacob[] = { 1, 3, 5, 11, 21, 43, 85, 171, 341, 683, 1365, 2731, 5461, 10923, 21845, 43691, 87381 };
	size_t last_inserted = 1; // pend[0] is done, so index 1 is next to be evaluated

	for (size_t j = 1; j < 13; ++j)
	{
		size_t target_idx = jacob[j] - 1; // Convert Jacobsthal number to 0-based array index
		
		// If the Jacobsthal index is beyond our pend size, cap it at the last element
		if (target_idx >= pend.size())
			target_idx = pend.size() - 1;

		// Insert backward from target_idx down to last_inserted
		for (size_t i = target_idx; i >= last_inserted; --i)
		{
			// Find where its original partner is currently sitting in sorted_main_chain
			size_t right_bound = sorted_main_chain.size();

			size_t insert_pos = _binarySearch(sorted_main_chain, pend[i], right_bound);
			
			sorted_main_chain.insert(sorted_main_chain.begin() + insert_pos, pend[i]);
			
			if (i == last_inserted)
				break; // Prevent underflow wrap-around with size_t
		}
		
		last_inserted = target_idx + 1;
		if (last_inserted >= pend.size())
			break;		
	}
	if (has_leftover)
	{
		size_t insert_pos = _binarySearch(sorted_main_chain, leftover, sorted_main_chain.size());
		sorted_main_chain.insert(sorted_main_chain.begin() + insert_pos, leftover);
	}


	return sorted_main_chain;
}

void PmergeMe::execute(int argc, char** argv)
{
	std::vector<PmergeMe::Element> input_vec = _parseInputToVector(argc, argv);

	std::cout << "UNSORTED Sequence: ";
	for (size_t i = 0; i < input_vec.size(); ++i)
		std::cout << input_vec[i].value << " ";
	std::cout << std::endl;

	std::vector<Element> sorted_vector = _fordJohnsonSort(input_vec);

	std::cout << "SORTED Sequence:  ";
	for (size_t i = 0; i < sorted_vector.size(); ++i)
		std::cout << sorted_vector[i].value << " ";
	std::cout << std::endl;
	
	// TO-DO: LATER ALLIGATOR
	// On the third line, you must display an explicit message indicating the time taken
	// by your algorithm, specifying the first container used to sort the positive integer
	// sequence.
	// On the last line you must display an explicit text indicating the time used by
	// your algorithm by specifying the second container used to sort the positive integer
	// sequence.
}
