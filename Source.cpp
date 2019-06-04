#include "Read.h"
#include "Command.h"
#include "Runner.h"

#include <iostream>
#include <fstream>

template<typename T, typename F>
bool try_file(const T& stream, F& file, const std::string& name);

int main(int argc, char *argv[]) {
	auto cmd = CommandParser(argc, argv);
	Options opt(CMDHelper::work_with_cmd(cmd));

	std::ofstream file_out;
	std::ifstream file_in;

	std::ostream* out = &std::cout;
	std::istream* in = &std::cin;

	if (try_file(out, file_out, opt.output_file)) {
		out = &file_out;
	}
	if (try_file(out, file_in, opt.input_file)) {
		in = &file_in;
	}

	if (opt.help) {
		Printer::print_help(*out);
		return 0;
	}

	try {
		if (opt.is_testing()) {
			std::function<size_t(size_t)> fn = [&opt](size_t index) -> size_t {
				return opt.get_index(index);
			};
			
			Printer::print_test("TSS", Runner::run_test(opt, *out, fn, true, opt.prefer_universal), fn, *out);
			Printer::print_test("Quine", Runner::run_test(opt, *out, fn, false, opt.prefer_universal), fn, *out);
		}
		else {
			Runner::run_program(opt, Read::read_file(*in), *out, false, opt.prefer_universal);
		}
	}
	catch (std::string e) {
		*out << "Program catches exception: \"" + e + "\". Contact to developers!\n\n";
		Printer::print_help(*out);
		return 1;
	}
	
	return 0;
}

template<typename T, typename F>
bool try_file(const T& stream, F& file, const std::string& name) {
	if (name != "") {
		file.open(name);
		if (!file.is_open()) {
			*stream << "Bad file \"" + name + "\"\n";
		}
		else {
			return true;
		}
	}

	return false;
}
