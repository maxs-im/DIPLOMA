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
		size_t (*convert)(size_t),
		bool is_linear,
		bool prefer_universal
	) throw (...) {
		// x - variables, y - equations
		std::vector<std::vector<double>> storage(opt.random.first, std::vector<double>(opt.random.second, -1));

		for (size_t v = 2; v <= opt.random.first; ++v) {
			for (size_t e = 1; e <= opt.random.second; ++e) {
				if (((size_t) 1 << v) >= e) {
					storage[v - 1][e - 1] = run_program(opt, Read::ex_generator(convert(v), convert(e), is_linear), out, true, prefer_universal);
				}
			}
		}

		return storage;
	}

}