#include "Read.h"
#include "Zhegalkin.h"
#include "Command.h"
#include <fstream>
#include <iostream>

void work_with_cmd(const CommandParser& cmd);

void print_answers(
	const std::set<std::string>& vocabulary,
	const std::vector<unsigned int>& answers,
	const std::string& title
);

int main(int argc, char *argv[]) {
	auto cmd = CommandParser(argc, argv);
	work_with_cmd(cmd);
	
	std::ifstream file("test.txt");
	auto x = Read::read_file(file);
	auto sys = System_Equations(x);
	/*
	for (const auto& it : sys.coefficients) {
		for (auto ii : it) {
			std::cout << ii << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\nVARIABLES\n";
	for (const auto& it : sys.vocabulary) {
		std::cout << it << " ";
	}
	std::cout << "\nERRORS\n";
	for (const auto& it : sys.errors) {
		std::cout << it.get_error() << "\n";
	}
	*/
	std::cout << "\n\n\t\tANSWERS\n";

	auto answers = sys.resolve();
	const auto&	individual(answers.first),
				basis(answers.second);

	if (!individual.size() && !basis.size()) {
		std::cout << "NO SOLUTIONS";
	}
	else {
		print_answers(sys.vocabulary, individual, "INDIVIDUAL");
		print_answers(sys.vocabulary, basis, "BASIS");
	}
	
	return 0;
}

void print_answers(
	const std::set<std::string>& vocabulary,
	const std::vector<unsigned int>& answers,
	const std::string& title
) {
	if (answers.size()) {
		std::cout << "\n" + title + ":\n";
		for (auto it = vocabulary.begin(); it != vocabulary.end(); ++it) {
			std::cout << *it + " \t";
		}
		std::cout << "\n";
		for (const auto s_it : answers) {
			int i = 0;
			for (auto it = vocabulary.begin(); it != vocabulary.end(); ++it, ++i) {
				std::cout << ((s_it & (1 << i)) ? 1 : 0) << " \t";
			}
			std::cout << "\n";
		}
	}
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
