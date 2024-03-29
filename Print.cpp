#include "Print.h"

namespace Printer {
	void print_logs(
		const System_Equations& sys,
		std::ostream& out
	) {
		if (sys.vocabulary.size()) {
			out << "\nVARIABLES\n";
			for (const auto& it : sys.vocabulary) {
				out << it << " ";
			}
		}
		if (sys.errors.size()) {
			out << "\nERRORS\n";
			for (const auto& it : sys.errors) {
				out << it.get_error() << "\n";
			}
		}
	}

	void print_answers(
		const std::pair<std::vector<size_t>, std::vector<size_t>>& answers,
		const std::set<std::string> vocabulary,
		std::ostream& out
	) {
		if (vocabulary.size()) {
			out << "\n\n\t\tANSWERS\n";
			const auto& individual(answers.first),
				basis(answers.second);

			if (!individual.size() && !basis.size()) {
				out << "NO SOLUTIONS. Maybe infinity amount\n";
			}
			else {
				print_block(vocabulary, individual, "INDIVIDUAL", out);
				print_block(vocabulary, basis, "BASIS", out);
			}
		}
		else {
			out << "\nChange condition! There is no any parsed variables\n";
		}
	}

	void print_block(
		const std::set<std::string>& vocabulary,
		const std::vector<size_t>& answers,
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
				size_t i = 0;
				for (auto it = vocabulary.begin(); it != vocabulary.end(); ++it, ++i) {
					out << ((s_it & ((size_t) 1 << i)) ? 1 : 0) << " \t";
				}
				out << "\n";
			}
		}
	}

	void print_help(std::ostream& out) {
		const char* text =
			"\tHow to use this program?\n"
			"Program is used to solve systems of (non-)linear (in-)homogeneous equations\n"
			"For linear we'll use TSS method. Otherwise, Quine's method\n"
			"\n\n\tFor more info your could use such flags:\n"
			"\t\t\"-h\" - to see this info\n"
			"\t\t\"-rt\" +- f\"v:e\"- start time testing. v,e - numbers of variables & equations\n"
			"\t\t\"-s\" - helper for testing. Get number of step. If > 0 -> add, if < 0 - multiply each step\n"
			"\t\t\"-t\" - to detect the execution time of the main functions\n"
			"\t\t\"-l\" - something like logs (errors, parsed variables etc.)\n"
			"\t\t\"-f/-o\" - input/output file directories accordingly\n\n"
			"\tHow to write correct input?\n"
			"You should to know some rules:\n"
			"\t1. Each line == equation. But empty line -> means stop for parsing\n"
			"\t2. There are two ways to write an equation:\n"
			"\t\ta. In common boolean form\n"
			"Example: \'x1 x2 x3 10110010\'.\n"
			"Read from left to right -> first \'1\' -> means x = (x1 x2 x3) = (0, 0, 0), f(x) = 1\n"
			"Note: n - variables means we should have table with 2^n elements in !one string (only \'1\', \'0\' symbols)\n"
			"Also, do not repeat variables like \'x1 x1 0100\'\n"
			"\t\tb. In Zhegalkin form\n"
			"Example: \'x1 x2 + x3 = 1\'\n"
			"\'+\' - means adding modulo two, \' \' between variables - means conjunction\n"
			"Write each symbol with space!. You have two special symbols \'+\' and \'=\'.\n"
			"Note: Symbol \'=\' must be at the end. After it you can set either \'1\' or \'0\' (by default on nothing)\n"
			"\n\n\t\t\tAuthored: 20.05.2019 by Maxim Galchenko (@maxs-im)\n"
			;
		out << text;
	}

	void print_time(
		const double creation_time,
		const double resolving_time,
		const bool is_full,
		std::ostream& out
	) {
		out << std::scientific;
		out << "\n\tTIMER INFO\n";
		if (is_full) {
			out << "Time for parsing input (without reading): " << creation_time << " seconds\n";
		}
		out << "Time for resolving system: " << resolving_time << " seconds\n";
	}

	void print_system(
		const System_Equations& sys,
		std::ostream& out
	) {
		if (sys.coefficients.size() == 0) {
			return;
		}
		const std::string title = "System of equations";
		out << "\t" << title << "\n";

		const std::vector<std::string> variables(sys.vocabulary.begin(), sys.vocabulary.end());
		const auto get_polynom = [&variables](const size_t num) -> std::string {
			std::string polyn;
			for (const auto index : System_Equations::get_set_bits(num)) {
				polyn += variables[index] + " ";
			}

			return polyn;
		};
		const size_t eq_num = sys.coefficients.size();
		const auto get_sep = [eq_num](const size_t i) -> char {
			if (i == (eq_num - 1)) return i ? '\\' : ' ';
			else if (i == 0) return '/';
			else return '|';
		};
	
		for (size_t i = 0; i < eq_num; ++i) {
			bool has_zero(false);
			std::string equation;
			for (const auto& it : sys.coefficients[i]) {
				if (it == 0) {
					has_zero = true;
					continue;
				}
				
				equation += std::string(1, Parsing::Actions::ADD) + " " + get_polynom(it);
			}
			equation += std::string(1, Parsing::Actions::EQUAL) + " " + 
				(has_zero ? std::string(1, Parsing::Actions::POSITIVE) : std::string(1, Parsing::Actions::NEGATIVE));

			// erase first less symbols (+ )
			equation.erase(equation.begin(), equation.begin() + 2);
			
			
			out << get_sep(i) << equation << "\n";
		}

		out << "\t" << std::string(title.length(), '-') << "\n";
	}

	void print_test(
		const std::string& header,
		const std::vector<std::vector<double>>& storage,
		std::function<size_t(size_t)> convert,
		std::ostream& out
	) {
		// Note: ignore useless case with 1 variable
		const bool ignore_v1(convert(1) == 1);
		if (storage.size() == 0 || 
			storage.front().size() == (ignore_v1 ? 1 : 0)
		) {
			return;
		}

		out << std::scientific;
		std::string separator("\t");
		out << "\t" << header << " (in seconds)\n";

		for (size_t e = 0; e <= storage.size(); ++e ) {
			if (e == 0) {
				out << "e/v\t";
				for (size_t v = ignore_v1 ? 2 : 1; v <= storage.front().size(); ++v) {
					out << separator << convert(v) << separator;
				}
			}
			else {
				out << convert(e) << "|\t";
				const auto& line = storage[e - 1];
				for (size_t v = ignore_v1 ? 1 : 0; v < line.size(); ++v) {
					if (line[v] < 0) {
						out << "\t#";
					}
					else {
						out << line[v];
					}
					out << separator;
				}
			}
			out << "\n\n";
		}
	}
}