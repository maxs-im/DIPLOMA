#pragma once
#include "Zhegalkin.h"
#include <iostream>
#include <iomanip>

namespace Printer {
	void print_help(std::ostream& out);

	void print_logs(
		const System_Equations& sys,
		std::ostream& out
	);

	void print_answers(
		const std::pair<std::vector<size_t>, std::vector<size_t>>& answers,
		const std::set<std::string> vocabulary,
		std::ostream& out
	);

	void print_block(
		const std::set<std::string>& vocabulary,
		const std::vector<size_t>& answers,
		const std::string& title,
		std::ostream& out
	);

	void print_time(
		const double creation_time,
		const double resolving_time,
		const bool is_full,
		std::ostream& out
	);

	void print_system(
		const System_Equations& sys,
		std::ostream& out
	);

	void print_test(
		const std::string& header,
		const std::vector<std::vector<double>>& time,
		size_t (*convert)(size_t),
		std::ostream& out
	);
}