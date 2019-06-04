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
	tokens.reserve((size_t) argc - 1);
	for (int i = 1; i < argc; ++i)
		tokens.push_back(std::string(argv[i]));
}

Options::Options() :
	help(false),
	logs(false),
	timer(false),
	prefer_universal(false),
	input_file(""),
	output_file(""),
	random(std::make_pair(0, 0)),
	step(0)
{}

Options CMDHelper::work_with_cmd(const CommandParser& cmd) {
	Options opt;
	// help
	opt.help = cmd.is_exists("-h");
	// errors logs
	opt.logs = cmd.is_exists("-l");
	// timer
	opt.timer = cmd.is_exists("-t");
	// universal
	opt.prefer_universal = cmd.is_exists("-u");

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

	//testing
	command = "-rt";
	if (cmd.is_exists(command)) {
		opt.set_test(cmd.get_value(command));
	}
	command = "-s";
	if (cmd.is_exists(command)) {
		try {
			opt.step = std::stoll(cmd.get_value(command));
		}
		catch (...) {}
	}

	return opt;
}

bool Options::is_testing() const {
	return random.first > 0 && random.second > 0;
}

void Options::set_test(const std::string& str) {
	std::string buffer = "";
	const char separator(':');

	for (size_t i = 0; i < str.length(); ++i) {
		if (str[i] != separator) {
			buffer += str[i];
		}
		if (str[i] == separator || (i + 1) == str.length()) {
			if (buffer != "") {
				try {
					size_t num = std::stoull(buffer);
					if (num == 0) {
						throw "";
					}

					if (random.first == 0) {
						random.first = num;
					}
					else if (random.second == 0) {
						random.second = num;
						break;
					}
				}
				catch (...) {}

				buffer = "";
			}
		}
	}

	// default values for flag
	if (random.first == 0) {
		random.first = 8;
	}
	if (random.second == 0) {
		random.second = 20;
	}
}

size_t Options::get_index(size_t index) const {
	if (step < 0) {
		return index * abs(step);
	}
	else {
		return index + step;
	}
}
