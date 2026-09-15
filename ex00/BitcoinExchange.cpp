/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanardh <djanardh@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 17:42:32 by djanardh          #+#    #+#             */
/*   Updated: 2026/09/15 02:25:23 by djanardh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() : _database() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& src)
{
	this->_database = src._database;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& src)
{
	if (this != &src)
		this->_database = src._database;

	return *this;
}

BitcoinExchange::~BitcoinExchange() {}

bool BitcoinExchange::_isValidDate(const std::string& date) const
{
	if (date.length() != 10 || date[4] != '-' || date[7] != '-')
		return false;

	int year = std::atoi(date.substr(0, 4).c_str());
	int month = std::atoi(date.substr(5, 2).c_str());
	int day = std::atoi(date.substr(8, 2).c_str());

	if (month < 1 || month > 12 || day < 1 || day > 31)
		return false;

	// Basic month day limits
	if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
		return false;
	if (month == 2)
	{
		bool isLeap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
		if (isLeap && day > 29)
			return false;
		if (!isLeap && day > 28)
			return false;
	}
	return true;
}

bool BitcoinExchange::loadDatabase(const std::string& dbPath)
{
	std::ifstream file(dbPath.c_str());
	if (!file.is_open()) {
		std::cerr << "Error: could not open database file" << std::endl;
		return false;
	}

	std::string line;
	std::getline(file, line); // Skip header line

	while (std::getline(file, line))
	{
		size_t delim = line.find(',');
		if (delim != std::string::npos)
		{
			std::string date = line.substr(0, delim);
			std::string rateStr = line.substr(delim + 1);
			float rate = std::atof(rateStr.c_str());
			_database[date] = rate; // Insert into map
		}
	}
	file.close();
	return true;
}

void BitcoinExchange::evaluateInput(const std::string& inputPath)
{
	std::ifstream file(inputPath.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open file." << std::endl;
		return;
	}

	std::string line;
	std::getline(file, line); // Skip "date | value" header

	while (std::getline(file, line))
	{
		if (line.empty())
			continue;

		size_t delim = line.find('|');
		if (delim == std::string::npos)
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}

		// Clean up whitespace around |
		std::string date = line.substr(0, delim);
		std::string valStr = line.substr(delim + 1);
		
		// Trim spaces
		date.erase(date.find_last_not_of(" \t\r\n") + 1);
		valStr.erase(0, valStr.find_first_not_of(" \t\r\n"));

		if (valStr.empty())
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}

		if (!_isValidDate(date))
		{
			std::cout << "Error: bad input => " << date << std::endl;
			continue;
		}

		char* endptr;
		double val = std::strtod(valStr.c_str(), &endptr);
		if (*endptr != '\0' && !std::isspace(*endptr))
		{
			std::cout << "Error: bad input => " << valStr << std::endl;
			continue;
		}
		if (val < 0)
		{
			std::cout << "Error: not a positive number." << std::endl;
			continue;
		}
		if (val > 1000)
		{
			std::cout << "Error: too large a number." << std::endl;
			continue;
		}

		// lower bound Search Matrix
		std::map<std::string, float>::const_iterator it = _database.lower_bound(date);
		
		if (it != _database.end() && it->first == date)	// Found exact match
			std::cout << date << " => " << val << " = " << (val * it->second) << std::endl;
		else
		{
			// Not exact match, lower bound gives the upper/next date, move back by 1 element
			if (it == _database.begin())
				std::cout << "Error: date is older than any record in database => " << date << std::endl;
			else
			{
				--it; // step back to lower date
				std::cout << date << " => " << val << " = " << (val * it->second) << std::endl;
			}
		}
	}
	file.close();
}
