#include "Read.h"
#include "Zhegalkin.h"
#include "Command.h"
#include <fstream>
#include <iostream>

void work_with_cmd(const CommandParser& cmd);

int main(int argc, char *argv[]) {
	auto cmd = CommandParser(argc, argv);
	work_with_cmd(cmd);
	
	std::ifstream file("test.txt");
	auto x = Read::read_file(file);
	auto sys = System_Equations(x);

	for (const auto& it : sys.coefficients) {
		for (auto ii : it) {
			std::cout << ii << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\nVARIABLES\n";
	for (const auto& it : sys.ivocabulary) {
		std::cout << it << " ";
	}

	std::cout << std::endl;
	
	return 0;
}

void work_with_cmd(const CommandParser& cmd) {
	// help
	if (cmd.is_exists("-h")) {
		std::cout << "HELP";
	}

	// errors logs
	if (cmd.is_exists("-l")) {
		std::cout << "\n	\n\tLogs\n";
	}

	// input file
	if (cmd.is_exists("-f")) {
		std::cout << "input file";
	}

	// output file
	if (cmd.is_exists("-o")) {
		std::cout << "output file\n";
	}

	// timer
	if (cmd.is_exists("-t")) {
		std::cout << "timer\n";
	}
}
