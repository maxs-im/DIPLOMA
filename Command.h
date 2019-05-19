#pragma once
#include <vector>
#include <string>

#include <fstream>
#include <iostream>

class CommandParser {
	std::vector<std::string> tokens;
public:
	CommandParser(int argc, char* argv[]);

	std::string get_value(const std::string& flag) const;
	bool is_exists(const std::string& flag) const;
};

struct Options {
	bool help, logs, timer;
	std::istream* input_file;
	std::ostream* output_file;

	Options();
};

namespace CMDHelper {
	Options work_with_cmd(const CommandParser& cmd);
}
