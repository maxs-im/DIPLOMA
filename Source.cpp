#include "Read.h"
#include "Zhegalkin.h"
#include "Command.h"
#include "Print.h"
#include "Timer.h"

int main(int argc, char *argv[]) {
	auto cmd = CommandParser(argc, argv);
	Options opt(CMDHelper::work_with_cmd(cmd));
		
	if (opt.help) {
		Printer::print_help(*opt.output_file);
		return 1;
	}

	auto x = Read::read_file(*opt.input_file);

	Timer time;
	auto sys = System_Equations(x);
	auto creation_time = time.elapsed();

	if (opt.logs) {
		Printer::print_logs(sys, *opt.output_file);
	}

	time.reset();
	auto answers = sys.resolve();
	auto resolving_time = time.elapsed();

	if (opt.timer) {
		Printer::print_time(creation_time, resolving_time, opt.logs, *opt.output_file);
	}

	Printer::print_answers(answers, sys.vocabulary, *opt.output_file);
	
	return 0;
}
