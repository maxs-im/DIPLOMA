#pragma once
#include "Parse.h"

#include <iostream>

class System_Equations : public Parsing {
	unsigned int num_queue(const std::string& var) const;
	
	unsigned int get_coef_from_combination(const std::vector<unsigned int>& vars);

	std::vector<unsigned int> get_set_bits(unsigned int n);

	std::vector<unsigned int> convert_table(const std::vector<std::string>& line);
	std::vector<unsigned int> convert_polynom(const std::vector<std::string>& line);

public:
	std::vector<std::vector<unsigned int>> coefficients;
	//std::vector<int> answers;

	System_Equations(const std::vector<std::vector<std::string>>& lines);
};


