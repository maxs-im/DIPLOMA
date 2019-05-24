#pragma once
#include "Read.h"
#include "Zhegalkin.h"
#include "Command.h"
#include "Print.h"
#include "Timer.h"

namespace Runner {
	double run_program(
		const Options& opt,
		const std::vector<std::vector<std::string>>& data,
		std::ostream& out,
		bool test_case = false
	) throw (...);

	std::vector<std::vector<double>> run_test(
		const Options& opt,
		std::ostream& out,
		bool is_linear = false
	) throw (...);
}