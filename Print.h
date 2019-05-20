#pragma once
#include "Zhegalkin.h"
#include <iostream>

namespace Printer {
	void print_help(std::ostream& out);

	void print_logs(
		const System_Equations& sys,
		std::ostream& out
	);

	void print_answers(
		const std::pair<std::vector<unsigned long long>, std::vector<unsigned long long>>& answers,
		const std::set<std::string> vocabulary,
		std::ostream& out
	);

	void print_block(
		const std::set<std::string>& vocabulary,
		const std::vector<unsigned long long>& answers,
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
}