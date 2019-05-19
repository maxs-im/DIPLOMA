#include "Zhegalkin.h"

using u_i = unsigned int;
template<typename T> using V = std::vector<T>;
using S = std::string;

u_i System_Equations::num_queue(const S& var) const {
	auto itr = vocabulary.begin();
	for (auto i = 0; itr != vocabulary.end(); ++i, ++itr) {
		if (*itr == var)
			return i;
	}

	return -1;
}

u_i System_Equations::get_coef_from_combination(const V<u_i> & vars) {
	u_i coef = 0;
	for (const auto& it : vars) {
		coef += (1 << it);
	}

	return coef;
}

V<u_i> System_Equations::get_set_bits(u_i n) {
	V<u_i> nums;
	u_i pos = 0;
	u_i mask = 1;

	while (n >= mask) {
		if (n & mask) {
			nums.push_back(pos);
		}
		mask <<= 1;
		++pos;
	}

	return nums;
}

V<u_i> System_Equations::convert_table(const V<S> & line) {
	V<u_i> coefs;
	coefs.reserve(line.size());
	const auto& table = line.back();

	for (u_i i = 0; i < table.length(); ++i) {
		if (table[i] == Actions::POSITIVE) {
			// b"10" = 2 (not 1)
			auto index = table.length() - i - 1;
			auto coef = get_coef_from_combination(get_set_bits(index));
			coefs.push_back(coef);
		}
	}

	return coefs;
}
V<u_i> System_Equations::convert_polynom(const V<S> & line) {
	V<u_i> coefs;
	V<u_i> combination;

	auto step_fn = [&]() {
		if (combination.size() > 0) {
			auto coef = get_coef_from_combination(combination);
			combination.clear();
			coefs.push_back(coef);
		}
	};

	for (const auto& var : line) {
		if (var == S(1, Actions::EQUAL)) {
			step_fn();
			break;
		}
		if (var == S(1, Actions::ADD)) {
			step_fn();
			continue;
		}

		combination.push_back(num_queue(var));
	}

	// only in case if value 1 ("... = 1") 
	if (line.back() == S(1, Actions::POSITIVE)) {
		coefs.push_back(0);
	}

	return coefs;
}

System_Equations::System_Equations(const V<V<S>>& lines)
	: Parsing(lines)
{
	coefficients.reserve(lines.size());

	for (const auto& equation : equations) {
		if (is_from_table(equation)) {
			coefficients.push_back(convert_table(equation));
		}
		else {
			coefficients.push_back(convert_polynom(equation));
		}
	}

	algo = std::make_unique<Solution>(coefficients, vocabulary.size());
}

std::pair<V<u_i>, V<u_i>> System_Equations::resolve() {
	return algo->solve();
}