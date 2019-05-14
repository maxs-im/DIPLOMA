#include "Read.h"
#include "Zhegalkin.h"
#include <fstream>

int main() {
	std::ifstream file("test.txt");
	auto x = Read::read_file(file);
	auto sys = System_Equations(x);
	return 0;
}
