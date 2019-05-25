#pragma once
#include <vector>
#include <string>
#include <utility>

class CommandParser {
	std::vector<std::string> tokens;
public:
	CommandParser(int argc, char* argv[]);

	std::string get_value(const std::string& flag) const;
	bool is_exists(const std::string& flag) const;
};

struct Options {
	bool help, logs, timer;
	// variables : system
	std::pair<size_t, size_t> random;
	std::string input_file;
	std::string output_file;

	Options();

	bool is_testing() const;
	void set_test(const std::string& str);
};

namespace CMDHelper {
	Options work_with_cmd(const CommandParser& cmd);
}
