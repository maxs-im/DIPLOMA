#include "Read.h"
#include <fstream>

int main() {
	std::ifstream file("test.txt");
	auto x = Read::read_file(std::cin);
	for (auto it : x) {
		for (auto iit : it)
			std::cout << iit << "\n";
		std::cout << "-------------\n";
	}
}
