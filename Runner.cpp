#include "Runner.h"

namespace Runner {
	double run_program(const Options& opt, const std::vector<std::vector<std::string>>& data, std::ostream& out, bool test_case, bool prefer_universal) throw (...) {
		Timer time;
		auto sys = System_Equations(data);
		auto creation_time = time.elapsed();

		time.reset();
		auto answers = sys.resolve(prefer_universal);
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

	std::vector<std::vector<double>> run_test(
		const Options& opt,
		std::ostream& out,
		std::function<size_t(size_t)> convert,
		bool is_linear,
		bool prefer_universal
	) throw (...) {
		// x - variables, y - equations
		std::vector<std::vector<double>> storage(opt.random.second, std::vector<double>(opt.random.first, -1));

		for (size_t v = 0; v < opt.random.first; ++v) {
			auto vars = convert(v + 1);
			size_t max_combinations = (size_t) 1 << vars;
			for (size_t e = 0; e < opt.random.second; ++e) {
				auto eqs = convert(e + 1);
				if (max_combinations >= eqs) {
					storage[e][v] = run_program(opt, Read::ex_generator(vars, eqs, is_linear), out, true, prefer_universal);
				}
			}
		}

		return storage;
	}

}