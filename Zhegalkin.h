#pragma once
#include "Parse.h"
#include "Algorithms.h"

class System_Equations : public Parsing {
protected:
	unsigned int num_queue(const std::string& var) const;
	
	unsigned int get_coef_from_combination(const std::vector<unsigned int>& vars);

	std::vector<unsigned int> get_set_bits(unsigned int n);

	std::vector<unsigned int> convert_table(const std::vector<std::string>& line);
	std::vector<unsigned int> convert_polynom(const std::vector<std::string>& line);

public:
	std::vector<std::vector<unsigned int>> coefficients;
	//Method* answers = nullptr;

	void solve() {
		//if (!answers) {
			auto algo = Solution(coefficients, vocabulary.size());

		//}
	}

	System_Equations(const std::vector<std::vector<std::string>>& lines);
};


