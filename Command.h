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

struct Options {
	bool help, logs, timer;
	std::string input_file;
	std::string output_file;

	Options();
};

namespace CMDHelper {
	Options work_with_cmd(const CommandParser& cmd);
}
