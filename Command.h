#pragma once
#include <vector>
#include <string>

class CommandParser {
	std::vector<std::string> tokens;
public:
	CommandParser(int argc, char* argv[]);

	std::string get_value(const std::string& flag) const;
	bool is_exists(const std::string& flag) const;
};