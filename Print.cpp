#include "Print.h"

namespace Printer {
	void Printer::print_logs(
		const System_Equations& sys,
		std::ostream& out
	) {
		out << "\nVARIABLES\n";
		for (const auto& it : sys.vocabulary) {
			out << it << " ";
		}
		out << "\nERRORS\n";
		for (const auto& it : sys.errors) {
			out << it.get_error() << "\n";
		}
	}

	void print_answers(
		const std::pair<std::vector<unsigned int>, std::vector<unsigned int>>& answers,
		const std::set<std::string> vocabulary,
		std::ostream& out
	) {
		out << "\n\n\t\tANSWERS\n";
		const auto& individual(answers.first),
			basis(answers.second);

		if (!individual.size() && !basis.size()) {
			out << "NO SOLUTIONS";
		}
		else {
			print_block(vocabulary, individual, "INDIVIDUAL", out);
			print_block(vocabulary, basis, "BASIS", out);
		}
	}

	void print_block(
		const std::set<std::string>& vocabulary,
		const std::vector<unsigned int>& answers,
		const std::string& title,
		std::ostream& out
	) {
		if (answers.size()) {
			out << "\n" + title + ":\n";
			for (auto it = vocabulary.begin(); it != vocabulary.end(); ++it) {
				out << *it + " \t";
			}
			out << "\n";
			for (const auto s_it : answers) {
				int i = 0;
				for (auto it = vocabulary.begin(); it != vocabulary.end(); ++it, ++i) {
					out << ((s_it & (1 << i)) ? 1 : 0) << " \t";
				}
				out << "\n";
			}
		}
	}

	// TODO: add small documentation
	void print_help(std::ostream & out) {
		const char* text =
			"\tHow to use this program?\n"
			""
			;
		out << text;
	}
}