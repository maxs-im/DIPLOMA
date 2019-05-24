#include "Runner.h"

namespace Runner {
	double run_program(const Options& opt, const std::vector<std::vector<std::string>>& data, std::ostream& out, bool test_case) throw (...) {
		Timer time;
		auto sys = System_Equations(data);
		auto creation_time = time.elapsed();

		time.reset();
		auto answers = sys.resolve();
		auto resolving_time = time.elapsed();

		if (opt.logs) {
			Printer::print_logs(sys, out);
		}

		if (opt.timer) {
			Printer::print_time(creation_time, resolving_time, opt.logs, out);
		}

		if (!test_case) {
			Printer::print_system(sys, out);
			Printer::print_answers(answers, sys.vocabulary, out);
		}

		return resolving_time;
	}

	std::vector<std::vector<double>> run_test(const Options& opt, std::ostream& out, bool is_linear) throw (...) {
		// x - variables, y - equations
		std::vector<std::vector<double>> storage;

		storage.reserve(opt.random.first);
		for (size_t v = 1; v <= opt.random.first; ++v) {
			storage[v - 1].reserve(opt.random.second);
			for (size_t e = 1; v <= opt.random.second; ++e) {
				auto time = run_program(opt, Read::ex_generator(v, e, is_linear), out, true);
				storage[v - 1].push_back(time);
			}
		}

		return storage;
	}

}