#include "Read.h"
#include "Zhegalkin.h"
#include "Command.h"
#include "Print.h"

int main(int argc, char *argv[]) {
	auto cmd = CommandParser(argc, argv);
	Options opt(CMDHelper::work_with_cmd(cmd));
		
	if (opt.help) {
		Printer::print_help(*opt.output_file);
		return 1;
	}

	auto x = Read::read_file(*opt.input_file);
	auto sys = System_Equations(x);

	if (opt.logs) {
		Printer::print_logs(sys, *opt.output_file);
	}

	auto answers = sys.resolve();

	Printer::print_answers(answers, sys.vocabulary, *opt.output_file);
	
	return 0;
}
