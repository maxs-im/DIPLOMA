#pragma once
#include "Parse.h"
#include "Algorithms.h"
#include <memory>

class System_Equations : public Parsing {
protected:
	unsigned int num_queue(const std::string& var) const;
	
	unsigned int get_coef_from_combination(const std::vector<unsigned int>& vars);

	std::vector<unsigned int> get_set_bits(unsigned int n);

	std::vector<unsigned int> convert_table(const std::vector<std::string>& line);
	std::vector<unsigned int> convert_polynom(const std::vector<std::string>& line);

	std::unique_ptr<Solution> algo;
public:
	std::vector<std::vector<unsigned int>> coefficients;

	std::vector<unsigned int> resolve() {
		// TODO: finish with named variables
		return algo->solve();
	}

	System_Equations(const std::vector<std::vector<std::string>>& lines);
};


