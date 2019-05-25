#pragma once
#include <string>
#include <vector>

#include <iostream>
#include <sstream>
#include <iterator>

#include <random>

#include "Zhegalkin.h"

namespace Read {
	std::vector<std::vector<std::string>> read_file(std::istream& stream) throw(...);

	std::vector<std::vector<std::string>> ex_generator(const size_t variables, const size_t equations, bool linear);
}