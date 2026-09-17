/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanardh <djanardh@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 13:52:24 by djanardh          #+#    #+#             */
/*   Updated: 2026/09/17 15:33:37 by djanardh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& src)
{
	(void)src;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& src)
{
	(void)src;
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

std::vector<size_t> PmergeMe::_generateJacobsthalSequence(size_t maxLimit)
{
	std::vector<size_t> jseq;
	jseq.push_back(0); // J_0 = 0
	jseq.push_back(1); // J_1 = 1

	// Generate values based on the last J value, not loop count!!
	while (jseq.back() < maxLimit)
	{
		size_t next_jn = jseq[jseq.size() - 1] + 2 * jseq[jseq.size() - 2];
		jseq.push_back(next_jn);
	}

	// erase 0 and first 1
	if (jseq.size() > 2)
		jseq.erase(jseq.begin(), jseq.begin() + 2);

	return jseq;
}

//////////// VECTOR ////////////

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
		// elem.backpack is empty by default when instantiated
		
		parsed_vector.push_back(elem);
	}

	if (parsed_vector.empty())
	{
		std::cerr << "Error: No numbers provided" << std::endl;
		std::exit(1);
	}

	return parsed_vector;
}

size_t PmergeMe::_binarySearchVector(const std::vector<Element>& chain, const Element& target, size_t right_bound)
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


std::vector<PmergeMe::Element> PmergeMe::_fordJohnsonSortVector(std::vector<PmergeMe::Element>& input_vec)
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

	std::vector<Element> mainChainVector;
	// Attach pend_elem to main_elem's backpack (so that the paired partners can be tracked with main_elem)
	for (size_t i = 0; i < pairs.size(); ++i)
	{
		pairs[i].main_elem.backpack.push_back(pairs[i].pend_elem);
		mainChainVector.push_back(pairs[i].main_elem);
	}

	// Recursive call
	std::vector<Element> sorted_main_chain = _fordJohnsonSortVector(mainChainVector);

	// Insertion
	std::vector<Element> pend;

	// get partner from each element in sorted main_chain
	for (size_t i = 0; i < sorted_main_chain.size(); ++i)
	{
		pend.push_back(sorted_main_chain[i].backpack.back()); // partner is the last element inside the backpack vector
		sorted_main_chain[i].backpack.pop_back(); // remove it when done
	}
	
	// freebie insertion
	sorted_main_chain.insert(sorted_main_chain.begin(), pend[0]);

	std::vector<size_t> jacob = _generateJacobsthalSequence(pend.size());
	size_t last_inserted = 1; // pend[0] is done, so index 1 is next to be evaluated

	for (size_t j = 1; j < jacob.size(); ++j)
	{
		size_t target_idx = jacob[j] - 1; // Convert Jacobsthal number to 0-based array index
		
		// If the Jacobsthal index is beyond pend size, cap it at the last element in pend
		if (target_idx >= pend.size())
			target_idx = pend.size() - 1;

		// Insert backward from target_idx down to last_inserted
		for (size_t i = target_idx; i >= last_inserted; --i)
		{
			// Find where its original partner is currently sitting in sorted_main_chain
			size_t right_bound = sorted_main_chain.size();

			size_t insert_pos = _binarySearchVector(sorted_main_chain, pend[i], right_bound);
			
			sorted_main_chain.insert(sorted_main_chain.begin() + insert_pos, pend[i]);
			
			if (i == last_inserted)
				break;
		}
		
		last_inserted = target_idx + 1;
		if (last_inserted >= pend.size())
			break;		
	}
	if (has_leftover)
	{
		size_t insert_pos = _binarySearchVector(sorted_main_chain, leftover, sorted_main_chain.size());
		sorted_main_chain.insert(sorted_main_chain.begin() + insert_pos, leftover);
	}


	return sorted_main_chain;
}

//////////// DEQUE ////////////

std::deque<PmergeMe::Element> PmergeMe::_parseInputToDeque(int argc, char** argv)
{
	std::deque<Element> parsed_deque;

	for (int i = 1; i < argc; ++i) {
		if (!_isValidNumber(argv[i]))
		{
			std::cerr << "Error: Invalid argument format" << std::endl;
			std::exit(1);
		}

		long checked_val = std::strtol(argv[i], NULL, 10);
		if (checked_val > INT_MAX)
		{
			std::cerr << "Error: Number exceeds INT_MAX limits" << std::endl;
			std::exit(1);
		}
		int clean_int = static_cast<int>(checked_val);

		for (size_t j = 0; j < parsed_deque.size(); ++j)
		{
			if (parsed_deque[j].value == clean_int)
			{
				std::cerr << "Error: Duplicate value detected" << std::endl;
				std::exit(1);
			}
		}

		Element elem;
		elem.value = clean_int;
		parsed_deque.push_back(elem);
	}
	return parsed_deque;
}

size_t PmergeMe::_binarySearchDeque(const std::deque<Element>& chain, const Element& target, size_t right_bound)
{
	size_t left = 0;
	size_t right = right_bound;

	while (left < right)
	{
		size_t mid = left + (right - left) / 2;
		if (chain[mid].value < target.value)
			left = mid + 1;
		else
			right = mid;
	}
	return left;
}

std::deque<PmergeMe::Element> PmergeMe::_fordJohnsonSortDeque(std::deque<Element>& input_deq)
{
	if (input_deq.size() <= 1)
		return input_deq;

	std::deque<ElementPair> pairs;
	Element leftover;
	bool has_leftover = false;

	for (size_t i = 0; i < input_deq.size(); i += 2)
	{
		if (i + 1 < input_deq.size())
		{
			ElementPair p;
			if (input_deq[i].value > input_deq[i + 1].value)
			{
				p.main_elem = input_deq[i];
				p.pend_elem = input_deq[i + 1];
			}
			else
			{
				p.main_elem = input_deq[i + 1];
				p.pend_elem = input_deq[i];
			}
			pairs.push_back(p);
		}
		else
		{
			leftover = input_deq[i];
			has_leftover = true;
		}
	}

	std::deque<Element> mainChainDeque;
	for (size_t i = 0; i < pairs.size(); ++i)
	{
		pairs[i].main_elem.backpack.push_back(pairs[i].pend_elem);
		mainChainDeque.push_back(pairs[i].main_elem);
	}

	std::deque<Element> sorted_main_chain = _fordJohnsonSortDeque(mainChainDeque);

	std::deque<Element> pend;
	for (size_t i = 0; i < sorted_main_chain.size(); ++i)
	{
		pend.push_back(sorted_main_chain[i].backpack.back());
		sorted_main_chain[i].backpack.pop_back();
	}

	sorted_main_chain.insert(sorted_main_chain.begin(), pend[0]);

	std::vector<size_t> jacob = _generateJacobsthalSequence(pend.size());
	size_t last_inserted = 1;

	for (size_t j = 1; j < jacob.size(); ++j)
	{
		size_t target_idx = jacob[j] - 1;
		if (target_idx >= pend.size())
			target_idx = pend.size() - 1;

		for (size_t i = target_idx; i >= last_inserted; --i)
		{
			size_t right_bound = sorted_main_chain.size();
			size_t insert_pos = _binarySearchDeque(sorted_main_chain, pend[i], right_bound);
			sorted_main_chain.insert(sorted_main_chain.begin() + insert_pos, pend[i]);
			
			if (i == last_inserted)
				break;
		}
		
		last_inserted = target_idx + 1;
		if (last_inserted >= pend.size())
			break;
	}

	if (has_leftover)
	{
		size_t insert_pos = _binarySearchDeque(sorted_main_chain, leftover, sorted_main_chain.size());
		sorted_main_chain.insert(sorted_main_chain.begin() + insert_pos, leftover);
	}

	return sorted_main_chain;
}


void PmergeMe::execute(int argc, char** argv)
{
	std::vector<PmergeMe::Element> input_vec = _parseInputToVector(argc, argv);

	std::cout << "Before: ";
	for (size_t i = 0; i < input_vec.size(); ++i)
		std::cout << input_vec[i].value << " ";
	std::cout << std::endl;

	std::clock_t start_clock_vector = std::clock();
	std::vector<Element> sorted_vector = _fordJohnsonSortVector(input_vec);
	std::clock_t end_clock_vector = std::clock();

	double time_taken_vector = static_cast<double>(end_clock_vector - start_clock_vector) / CLOCKS_PER_SEC * 1000000.0;

	std::deque<PmergeMe::Element> input_deq = _parseInputToDeque(argc, argv);

	std::clock_t start_clock_deque = std::clock();
	std::deque<Element> sorted_deque = _fordJohnsonSortDeque(input_deq);
	std::clock_t end_clock_deque = std::clock();

	double time_taken_deque = static_cast<double>(end_clock_deque - start_clock_deque) / CLOCKS_PER_SEC * 1000000.0;

	std::cout << "After:  ";
	for (size_t i = 0; i < sorted_vector.size(); ++i)
		std::cout << sorted_vector[i].value << " ";
	std::cout << std::endl;

	std::cout << std::fixed << std::setprecision(5);
	std::cout << "Time to process a range of " << input_vec.size() 
			<< " elements with std::vector : " << time_taken_vector << " us" << std::endl;
	std::cout << "Time to process a range of " << input_deq.size() 
			<< " elements with std::deque : " << time_taken_deque << " us" << std::endl;

}
