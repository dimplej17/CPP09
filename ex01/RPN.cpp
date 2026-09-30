/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanardh <djanardh@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 17:54:07 by djanardh          #+#    #+#             */
/*   Updated: 2026/09/30 14:33:01 by djanardh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

RPN::RPN() : _list() {}

RPN::RPN(const RPN& src)
{
	this->_list = src._list;
}

RPN& RPN::operator=(const RPN& src)
{
	if (this != &src)
		this->_list = src._list;
	return *this;
}

RPN::~RPN() {}

bool RPN::_isOperator(char c) const
{
	return (c == '+' || c == '-' || c == '*' || c == '/');
}

bool RPN::_performOperation(char op)
{
	// An operation requires at least two numbers inside the list
	if (_list.size() < 2)
		return false;

	// The first popped element is the RIGHT operand (b)
	int b = _list.back();
	_list.pop_back();

	// The second popped element is the LEFT operand (a)
	int a = _list.back();
	_list.pop_back();

	int result = 0;
	if (op == '+')
		result = a + b;
	else if (op == '-')
		result = a - b;
	else if (op == '*')
		result = a * b;
	else if (op == '/')
	{
		if (b == 0)
		{
			std::cerr << "Error: division by zero" << std::endl;
			return false;
		}
		result = a / b;
	}

	_list.push_back(result);
	return true;
}

void RPN::calculate(const std::string& expression)
{
	for (size_t i = 0; i < expression.length(); i++)
	{
		char c = expression[i];

		// Skip spaces cleanly
		if (std::isspace(c))
			continue;

		if (std::isdigit(c)) // Convert character digit ('0'-'9') to integer value
			_list.push_back(c - '0');
		else if (_isOperator(c))
		{
			if (!_performOperation(c))
			{
				std::cerr << "Error" << std::endl;
				return;
			}
		} 
		else
		{
			std::cerr << "Error: Brackets, letters, or invalid tokens" << std::endl;
			return;
		}
	}

	// only one final result should remain
	if (_list.size() != 1)
	{
		std::cerr << "Error: more than 1 number remaining in the list" << std::endl;
		return;
	}

	std::cout << _list.back() << std::endl;
}
