#pragma once
#include <string>
#include <vector>

#include <iostream>
#include <sstream>
#include <iterator>

namespace Read {
	std::vector<std::vector<std::string>> read_file(std::istream& stream) throw(...);
}