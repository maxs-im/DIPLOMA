#pragma once
#include "Parse.h"

#include <iostream>

class System_Equations : protected Parsing {
public:
	std::vector<unsigned int> coefficients;

	System_Equations(const std::vector<std::vector<std::string>>& lines) : Parsing(lines) {
		for (const auto& it : errors)
			std::cout << it.get_error() + "\n";
		for (const auto& it : vocabulary)
			std::cout << it << "\n";
	}
};