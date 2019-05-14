#pragma once
#include <vector>
#include <string>
#include <bitset>
#include <set>
#include <cmath>
#include <unordered_set>

#include <iostream>

class Parsing {
	enum Errors {
		BIT = -1,
		NONE,
		IGNORE, INCOMPATIBILITY,
		GENERAL, NUMBER, DUPLICATE,
		EMPTY
	};

	std::string get_error(int code);

	int parse_table(const std::vector<std::string>& tokens);
	int parse_polynom(const std::vector<std::string>& tokens);
	int parse_line(const std::vector<std::string>& line);

protected:
	enum Actions {
		EQUAL = '=',
		ADD = '+',
		NEGATIVE = '0',
		POSITIVE = '1'
	};

public:
	// storage for variable names
	std::set<std::string> vocabulary;
	// valid equations
	std::vector<std::vector<std::string>> equations;

	Parsing(const std::vector<std::vector<std::string>>& lines);
};

class System_Equations : protected Parsing {
public:
	std::vector<unsigned int> coefficients;

	System_Equations(const std::vector<std::vector<std::string>>& lines) : Parsing(lines) {
		for (const auto& it : vocabulary)
			std::cout << it << "\n";
	}
};