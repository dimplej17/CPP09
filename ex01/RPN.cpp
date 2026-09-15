/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanardh <djanardh@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 17:54:07 by djanardh          #+#    #+#             */
/*   Updated: 2026/09/15 02:18:36 by djanardh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

RPN::RPN() : _stack() {}

RPN::RPN(const RPN& src)
{
	this->_stack = src._stack;
}

RPN& RPN::operator=(const RPN& src)
{
	if (this != &src)
		this->_stack = src._stack;
	return *this;
}

RPN::~RPN() {}

bool RPN::_isOperator(char c) const
{
	return (c == '+' || c == '-' || c == '*' || c == '/');
}

bool RPN::_performOperation(char op)
{
	// An operation requires at least two numbers inside the stack
	if (_stack.size() < 2)
		return false;

	// The first popped element is the RIGHT operand (b)
	int b = _stack.top();
	_stack.pop();

	// The second popped element is the LEFT operand (a)
	int a = _stack.top();
	_stack.pop();

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

	_stack.push(result);
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
			_stack.push(c - '0');
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
	if (_stack.size() != 1)
	{
		std::cerr << "Error: more than 1 number remaining in the stack" << std::endl;
		return;
	}

	std::cout << _stack.top() << std::endl;
}
