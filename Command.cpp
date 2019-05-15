#include "Command.h"

std::string CommandParser::get_value(const std::string& flag) const {
	auto itr = std::find(tokens.begin(), tokens.end(), flag);
	if (itr != tokens.end() && ++itr != tokens.end()) {
		return *itr;
	}

	return "";
}

bool CommandParser::is_exists(const std::string& flag) const {
	return std::find(tokens.begin(), tokens.end(), flag) != tokens.end();
}

CommandParser::CommandParser(int argc, char* argv[]) {
	tokens.reserve(argc - 1);
	for (int i = 1; i < argc; ++i)
		tokens.push_back(std::string(argv[i]));
}