#include "Algorithms.h"
#include "templates.h"

std::pair<V<u_i>, V<u_i>> Solution::solve() {
	std::pair<V<u_i>, V<u_i>> vectors;

	if (linear) {
		auto obj = TSS(this);

		vectors = obj.solve();
	}
	else {
		auto obj = Quine(this);

		vectors = obj.solve();
	}

	return vectors;
}

bool Solution::is_linear(const V<V<u_i>>& _coefficients) {
	for (const auto& l_it : _coefficients) {
		for (const auto& it : l_it) {
			if (it & (it - 1))
				return false;
		}
	}

	return true;
}

Solution::Solution(const V<V<u_i>> & _coefficients, const u_i _range) :
	coefficients(_coefficients),
	range(_range),
	linear(Solution::is_linear(_coefficients))
{
	for (const auto& l_it : coefficients)
		for (const auto& it : l_it)
			if (it > ((unsigned int) 1 << range))
				throw "Range is incorrect for Solution";
}

void Method::reduce(V<u_i>& equation) {
	std::unordered_set<u_i> buff;
	for (const auto& it : equation) {
		auto look = buff.find(it);
		if (look == buff.end()) {
			buff.insert(it);
		}
		else {
			buff.erase(look);
		}
	}

	// if changed
	if (buff.size() != equation.size())
		equation = V<u_i>(buff.begin(), buff.end());
}

void Method::invalid_call() const throw(...) {
	throw "Invalid method choice. Contact to developers!";
}

Method::Method(const Solution * _data) : data_ptr(_data) {
	if (data_ptr == nullptr)
		throw "Inccorect pointer. Contact to developers!";
};
	
std::pair<V<u_i>, V<u_i>> Method::solve() {
	return std::make_pair(answers, basis);
}


Quine::Value::Value(u_i _index, bool _positive) :
	index(_index),
	positive(_positive)
{}

int Quine::Value::count(const u_i coef) const {
	u_i number = get_number();
	if (positive) {
		return coef & (~number);
	}
	else {
		return (coef & number) ? -1 : coef;
	}
}


u_i operator+ (const Quine::Value& val, const u_i vec) {
	auto number = val.get_number();
	return val.positive ? (vec | number) : (vec & (~number));
}

u_i Quine::Value::get_number() const {
	return 1 << index;
}

bool Quine::update_coefs(V<V<u_i>>& coefs, const Value& val) {
	for (int li = 0; li < coefs.size(); ++li) {
		V<u_i>& equation = coefs[li];

		for (int i = 0; i < equation.size(); ++i) {
			int new_val = val.count(equation[i]);
			if (new_val == -1) {
				// polynom = zero
				equation.erase(equation.begin() + i);
				if (i > 0) i--;
			}
			else {
				equation[i] = new_val;
			}
		}
		reduce(equation);

		// incompatibility
		if ((equation.size() == 1) && equation[0] == 0) {
			// 1 = 0
			return false;
		}
		if (equation.size() == 0) {
			coefs.erase(coefs.begin() + li);
			li--;
		}
	}

	return true;
}

void Quine::set_value(V<V<u_i>> coefs, const u_i vec, const Value & val) {
	if (update_coefs(coefs, val)) {
		step(coefs, val + vec);
	}
}

// find first unknown variable in current @coefs
u_i Quine::get_nearest_var(const V<V<u_i>> & coefs) {
	for (const auto& l_it : coefs) {
		for (const auto& it : l_it) {
			if (it > 0) {
				u_i pos = 0;
				u_i mask = 1;

				while (!(mask & it)) {
					mask <<= 1;
					++pos;
				}

				return pos;
			}
		}
	}

	return -1;
}

// like dfs
void Quine::step(const V<V<u_i>> & coefs, u_i vec) {
	if (coefs.empty()) {
		// add new solution
		answers.push_back(vec);
		return;
	}

	int index = get_nearest_var(coefs);
	if (index == -1) {
		// UNREACHABLE
		return;
	}
	// Positive
	set_value(coefs, vec, Value(index, true));
	// Negative
	set_value(coefs, vec, Value(index, false));
}

// Note: zero coefficient means Positive value for equation
Quine::Quine(const Solution * solution) : Method(solution) {
	if (data_ptr->linear)
		invalid_call();
}

std::pair<V<u_i>, V<u_i>> Quine::solve() {
	answers.clear();
	basis.clear();

	step(data_ptr->coefficients);
	return std::make_pair(answers, basis);
}

V<V<u_i>> TSS::prepare_coefs(V<V<u_i>> coefs, const u_i range) throw(...) {
	for (int i = 0; i < coefs.size(); ++i) {
		auto& equation = coefs[i];
		reduce(equation);
		if ((equation.size() == 1) && equation[0] == 0) {
			// 1 = 0
			throw "Incorrect parsing for TSS. Contact to developers";
		}
		if (equation.size() == 0) {
			coefs.erase(coefs.begin() + i);
			--i;
		}

		// replace value = 1 (b_i) with one more variable
		for (auto& it : equation) {
			if (it == 0) {
				it = 1 << range;
			}
		}
	}

	return coefs;
}

V<u_i> TSS::get_start_vectors(const V<u_i> & first, u_i range) {
	V<u_i> vectors;
	vectors.reserve(range - 1);

	u_i pillar = first.front();
	for (u_i j = 1; j < range; ++j) {
		auto eq_pillar = 1 << j;
		u_i lor;
		if (std::find(first.begin(), first.end(), eq_pillar) != first.end()) {
			lor = pillar + eq_pillar;
		}
		else {
			lor = eq_pillar;
		}
		vectors.push_back(lor);
	}

	return vectors;
}

bool TSS::multiply(const V<u_i> & equation, const u_i value) {
	bool ans = false;
	for (const auto& it : equation) {
		if (value & it) {
			ans = !ans;
		}
	}

	return ans;
}

u_i TSS::calcul_sum(const V<u_i> & coefs, const u_i vec) {
	u_i ans = 0;
	for (u_i i = 0; i < coefs.size(); ++i) {
		if ((1 << i) & vec) {
			ans ^= coefs[i];
		}
	}
	return ans;
}

V<u_i> TSS::add_equation(const V<u_i> & equation, const V<u_i> & vectors) {
	V<u_i> tmp_coef;
	tmp_coef.reserve(vectors.size());

	for (u_i i = 0; i < vectors.size(); ++i) {
		if (multiply(equation, vectors[i])) {
			tmp_coef.push_back(1 << i);
		}
	}
	if (tmp_coef.size() == 0) {
		// same equations (prev + current)
		return equation;
	}

	V<u_i>	tmp_vec(get_start_vectors(tmp_coef, vectors.size())),
		ans;
	ans.reserve(tmp_vec.size());
	for (const auto& it : tmp_vec) {
		ans.push_back(calcul_sum(vectors, it));
	}

	return ans;
}


TSS::TSS(const Solution * solution) : Method(solution) {
	if (!data_ptr->linear)
		invalid_call();
}

// first -> individual, second -> basis
std::pair<V<u_i>, V<u_i>> TSS::separate_solutions(const V<u_i> & vec) {
	u_i null_value = 1 << data_ptr->range;
	answers.clear();
	basis.clear();

	for (const auto it : vec) {
		if (it & null_value) {
			answers.push_back(it & (~null_value));
		}
		else {
			basis.push_back(it);
		}
	}
	return std::make_pair(answers, basis);
}

std::pair<V<u_i>, V<u_i>> TSS::solve() {
	auto upd_coefs = prepare_coefs(data_ptr->coefficients, data_ptr->range);
	if (upd_coefs.size() == 0) {
		return { { 0 } , {} };
	}

	auto set_vec = get_start_vectors(upd_coefs.front(), data_ptr->range + 1);
	for (u_i i = 1; i < upd_coefs.size(); ++i) {
		set_vec = add_equation(upd_coefs[i], set_vec);
	}

	return separate_solutions(set_vec);
}