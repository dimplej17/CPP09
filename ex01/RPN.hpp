/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanardh <djanardh@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 17:54:12 by djanardh          #+#    #+#             */
/*   Updated: 2026/09/14 17:54:14 by djanardh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <stack>
#include <string>
#include <sstream>

class RPN
{
	private:
	std::stack<int> _stack;

	bool _isOperator(char c) const;
	bool _performOperation(char op);

	public:
	RPN();
	RPN(const RPN& src);
	RPN& operator=(const RPN& src);
	~RPN();

	void calculate(const std::string& expression);
};

#endif
