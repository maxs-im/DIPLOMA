#pragma once
#include "Parse.h"
#include "Algorithms.h"
#include <memory>

class System_Equations : public Parsing {
protected:
	unsigned long long num_queue(const std::string& var) const;
	std::vector<unsigned long long> convert_to_real(
		std::vector<unsigned long long> bits,
		const std::vector<std::string>& vars
	);
	
	unsigned long long get_coef_from_combination(const std::vector<unsigned long long>& vars);

	std::vector<unsigned long long> convert_table(const std::vector<std::string>& line);
	std::vector<unsigned long long> convert_polynom(const std::vector<std::string>& line);

	std::unique_ptr<Solution> algo;
public:
	std::vector<std::vector<unsigned long long>> coefficients;

	std::pair<std::vector<unsigned long long>, std::vector<unsigned long long>> resolve();

	System_Equations(const std::vector<std::vector<std::string>>& lines);

	static std::vector<unsigned long long> get_set_bits(unsigned long long n);
};


