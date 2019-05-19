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

Options::Options() :
	help(false),
	logs(false),
	timer(false),
	input_file(""),
	output_file("")
{}
Options CMDHelper::work_with_cmd(const CommandParser& cmd) {
	Options opt;
	// help
	opt.help = cmd.is_exists("-h");
	// errors logs
	opt.logs = cmd.is_exists("-l");
	// timer
	opt.timer = cmd.is_exists("-t");

	std::string command;
	// input file
	command = "-f";
	if (cmd.is_exists(command)) {
		opt.input_file = cmd.get_value(command);
	}
	// output file
	command = "-o";
	if (cmd.is_exists(command)) {
		opt.output_file = cmd.get_value(command);
	}

	return opt;
}
