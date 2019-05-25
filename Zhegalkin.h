#pragma once
#include "Parse.h"
#include "Algorithms.h"
#include <memory>

class System_Equations : public Parsing {
protected:
	size_t num_queue(const std::string& var) const throw (...);
	std::vector<size_t> convert_to_real(
		std::vector<size_t> bits,
		const std::vector<std::string>& vars
	);
	
	size_t get_coef_from_combination(const std::vector<size_t>& vars);

	std::vector<size_t> convert_table(const std::vector<std::string>& line);
	std::vector<size_t> convert_polynom(const std::vector<std::string>& line);

	std::unique_ptr<Solution> algo;
public:
	std::vector<std::vector<size_t>> coefficients;

	std::pair<std::vector<size_t>, std::vector<size_t>> resolve();

	System_Equations(const std::vector<std::vector<std::string>>& lines);

	static std::vector<size_t> get_set_bits(size_t n);
};


