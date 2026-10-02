/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanardh <djanardh@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 17:42:32 by djanardh          #+#    #+#             */
/*   Updated: 2026/10/02 12:18:09 by djanardh         ###   ########.fr       */
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

std::string BitcoinExchange::_trim(const std::string& s)
{
	size_t start = s.find_first_not_of(" \t\r\n");
	if (start == std::string::npos)
		return ""; // string was empty or only whitespace
	size_t end = s.find_last_not_of(" \t\r\n");
	return s.substr(start, end - start + 1);
}

bool BitcoinExchange::_isValidDate(const std::string& date) const
{
	if (date.length() != 10 || date[4] != '-' || date[7] != '-')
		return false;

	for (size_t i = 0; i < date.length(); ++i)
	{
		if (i == 4 || i == 7)
			continue;
		if (!std::isdigit(static_cast<unsigned char>(date[i])))
			return false;
	}
	
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

bool BitcoinExchange::_isValidValue(const std::string& s)
{
	if (s.empty()) return false;
	size_t i = 0;
	if (s[i] == '-' || s[i] == '+') ++i;
	bool digits = false, dot = false;
	for (; i < s.length(); ++i)
	{
		if (std::isdigit(static_cast<unsigned char>(s[i]))) digits = true;
		else if (s[i] == '.' && !dot) dot = true;
		else return false;
	}
	return digits;
}

bool BitcoinExchange::loadDatabase(const std::string& dbPath)
{
	std::ifstream file(dbPath.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open database file" << std::endl;
		return false;
	}
 
	_database.clear();
 
	std::string line;
	bool firstLine = true;
 
	while (std::getline(file, line))
	{
		line = _trim(line);
 
		// Skip the header only if it really is the header
		if (firstLine)
		{
			firstLine = false;
			if (line == "date,exchange_rate")
				continue;
		}
 
		if (line.empty())
			continue;
 
		size_t delim = line.find(',');
		if (delim == std::string::npos)
		{
			std::cerr << "Error: invalid database line => " << line << std::endl;
			return false;
		}
 
		std::string date = _trim(line.substr(0, delim));
		std::string rateStr = _trim(line.substr(delim + 1));
 
		if (!_isValidDate(date) || !_isValidValue(rateStr) || rateStr[0] == '-')
		{
			std::cerr << "Error: invalid database line => " << line << std::endl;
			return false;
		}
 
		_database[date] = std::strtod(rateStr.c_str(), NULL);
	}
 
	if (_database.empty())
	{
		std::cerr << "Error: database is empty" << std::endl;
		return false;
	}
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
	bool firstLine = true;
	bool sawAnyLine = false;

	while (std::getline(file, line))
	{
		sawAnyLine = true;

		if (firstLine)
		{
			firstLine = false;
			if (_trim(line) == "date | value")
				continue;
		}

		if (_trim(line).empty())
			continue;

		size_t delim = line.find('|');
		if (delim == std::string::npos)
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}

		std::string date = _trim(line.substr(0, delim));
		std::string valStr = _trim(line.substr(delim + 1));

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
		if (!_isValidValue(valStr))
		{
			std::cout << "Error: bad input => " << valStr << std::endl;
			continue;
		}

		double val = std::strtod(valStr.c_str(), NULL);
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

		std::map<std::string, double>::const_iterator it = _database.lower_bound(date);

		if (it != _database.end() && it->first == date)
			std::cout << date << " => " << val << " = " << (val * it->second) << std::endl;
		else
		{
			if (it == _database.begin())
				std::cout << "Error: date is older than any record in database => " << date << std::endl;
			else
			{
				--it;
				std::cout << date << " => " << val << " = " << (val * it->second) << std::endl;
			}
		}
	}

	if (!sawAnyLine)
		std::cout << "Error: input file is empty." << std::endl;

	file.close();
}

