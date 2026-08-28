
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

	// Helper validation functions
	bool isValidDate(const std::string& date) const;
	bool isValidValue(const std::string& valueStr, float& value) const;

	public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange& src);
	BitcoinExchange& operator=(const BitcoinExchange& src);
	~BitcoinExchange();

	bool loadDatabase(const std::string& dbPath);
	void evaluateInput(const std::string& inputPath);
};


#endif