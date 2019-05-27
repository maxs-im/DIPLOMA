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
		bool test_case = false,
		bool prefer_universal = false
	) throw (...);

	std::vector<std::vector<double>> run_test(
		const Options& opt,
		std::ostream& out,
		size_t (*convert)(size_t),
		bool is_linear = false,
		bool prefer_universal = false
	) throw (...);
}