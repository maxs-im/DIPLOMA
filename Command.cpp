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
	input_file(&std::cin),
	output_file(&std::cout)
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
		std::ifstream file(cmd.get_value(command));
		if (file.is_open()) {
			opt.input_file = &file;
		}
		else {
			*opt.output_file << "Can not open input file!\n";
		}
	}
	// output file
	command = "o";
	if (cmd.is_exists(command)) {
		std::ofstream file(cmd.get_value(command));
		if (file.is_open()) {
			opt.output_file = &file;
		}
		else {
			*opt.output_file << "Can not open output file!\n";
		}
	}

	return opt;
}