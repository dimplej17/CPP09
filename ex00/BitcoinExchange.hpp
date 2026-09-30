/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanardh <djanardh@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 17:42:43 by djanardh          #+#    #+#             */
/*   Updated: 2026/09/30 14:57:13 by djanardh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINGEXCHANGE_HPP
#define BITCOINGEXCHANGE_HPP

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>

class BitcoinExchange
{
	private:
	std::map<std::string, float> _database;

	bool _isValidDate(const std::string& date) const;
	bool _isValidValue(const std::string& s);
	public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange& src);
	BitcoinExchange& operator=(const BitcoinExchange& src);
	~BitcoinExchange();

	bool loadDatabase(const std::string& dbPath);
	void evaluateInput(const std::string& inputPath);
};


#endif