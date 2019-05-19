#include "Read.h"
#include "Zhegalkin.h"
#include "Command.h"

void print_answers(
	const std::set<std::string>& vocabulary,
	const std::vector<unsigned int>& answers,
	const std::string& title
);

int main(int argc, char *argv[]) {
	auto cmd = CommandParser(argc, argv);
	Options opt(CMDHelper::work_with_cmd(cmd));
	
	auto x = Read::read_file(*opt.input_file);
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
