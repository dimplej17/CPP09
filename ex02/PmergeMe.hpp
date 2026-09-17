/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanardh <djanardh@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 13:52:31 by djanardh          #+#    #+#             */
/*   Updated: 2026/09/16 15:28:48 by djanardh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <cstdlib> // For std::strtol
#include <climits> // For INT_MAX
#include <ctime> // for clock()
#include <iomanip>
#include <deque>

class PmergeMe
{
	public:
	// internal struct
	struct Element
	{
		int value;
		std::vector<Element> backpack;
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
	std::vector<Element> _fordJohnsonSortVector(std::vector<Element>& input_vec);
	size_t _binarySearchVector(const std::vector<Element>& chain, const Element& target, size_t right_bound);

	std::deque<Element> _parseInputToDeque(int argc, char** argv);
	std::deque<Element> _fordJohnsonSortDeque(std::deque<Element>& input_deq);
	size_t _binarySearchDeque(const std::deque<Element>& chain, const Element& target, size_t right_bound);
		
	bool _isValidNumber(const char* str) const;
};

#endif