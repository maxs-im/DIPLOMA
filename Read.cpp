#include "Read.h"
#include "templates.h"

using VS = V<S>;
using VVS = V<V<S>>;

VS split_string(const S& str) {
	std::istringstream buf(str);
	std::istream_iterator<S> beg(buf), end;

	return VS(beg, end);
}

VVS Read::read_file(std::istream& stream) throw(...) {
	VVS strlines(0);
	S str;
	if (!stream) {
		throw "Bad stream (check file)";
	}

	while (stream) {
		getline(stream, str);
		if (str == "")
			break;
		strlines.push_back(split_string(str));
	}

	return strlines;
}

VVS Read::ex_generator(const size_t variables, const size_t equations, bool linear) {
	VS template_equation(variables + 1);
	// generate var names
	for (size_t i = 0; i < variables; ++i) {
		template_equation[i] = "x" + std::to_string(i);
	}
	// last table value
	auto max_size = (size_t) 1 << (variables + 1);
	template_equation[variables] = S(max_size >> 1, '0');

	VVS answer(equations, template_equation);
	std::mt19937_64 generator(std::random_device{}());

	for (auto& eq : answer) {
		S& table_value = eq[variables];
		auto num = generator() % max_size;
		for (const auto& it : System_Equations::get_set_bits(num)) {
			table_value[it] = '1';
		}

	}

	return answer;
}
