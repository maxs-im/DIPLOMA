#pragma once
#include <vector>
#include <functional>
#include <unordered_set> 

template<typename T> using V = std::vector<T>;
using u_i = unsigned int;

class Solution {
	static bool is_linear(const V<V<u_i>>& _coefficients) {
		for (const auto& l_it : _coefficients) {
			for (const auto& it : l_it) {
				if ((it & (it - 1)) != 0)
					return false;
			}
		}

		return true;
	}
public:
	const V<V<u_i>>& coefficients;
	const u_i range;
	const bool linear;

	Solution(const V<V<u_i>>& _coefficients, const u_i _range): 
		coefficients(_coefficients), 
		range(_range), 
		linear(Solution::is_linear(_coefficients))
	{
		for (const auto& l_it : coefficients)
			for (const auto& it : l_it)
				if (it > (1 << range))
					throw "Range is incorrect for Solution";
	}

	V<u_i> solve();
};

// interface for our algorithms
class Method {
protected:
	const Solution* data_ptr;
	void invalid_call() const throw(...) {
		throw "Invalid method choice. Contact to developers!";
	}
public:
	Method(const Solution* _data) : data_ptr(_data) {
		if (data_ptr == nullptr)
			throw "Inccorect pointer. Contact to developers!";
	};
	V<u_i> answers; 
	virtual V<u_i> solve() {
		return answers;
	}

};

class Quine : public Method {
	void reduce(V<u_i>& equation) {
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

	struct Value {
		const u_i index;
		const bool positive;

		Value(u_i _index, bool _positive = true) :
			index(_index),
			positive(_positive)
		{}

		int count(const u_i coef) const {
			if (coef == 0) {
				return -1;
			}

			u_i number = get_number();
			if (positive) {
				auto new_val = (coef & (~number));
				// -1 - means add positive value
				return (new_val == number) ? -1 : new_val;
			}
			else {
				return ((coef & number) > 0) ? 0 : coef;
			}
		}

		friend u_i operator+ (const Value& val, const u_i vec) {
			auto number = val.get_number();
			return val.positive ? (vec | number) : (vec & number);
		}

	private:
		u_i get_number() const {
			return 1 << index;
		}
	};

	bool update_coefs(V<V<u_i>>& coefs, const Value& val) {
		for (int li = 0; li < coefs.size(); ++li) {
			V<u_i>& equation = coefs[li];
			
			for (int i = 0; i < equation.size(); ++i) {
				int new_val = val.count(equation[i]);
				switch (new_val)
				{
				// polynom X_n -> X_n set 1 -> get 1
				case -1: 
					equation[i] = 0;
					break;
				case 0: 
					equation.erase(equation.begin() + i);
					i--;
					break;
				default:
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

	void set_value(V<V<u_i>> coefs, const u_i vec, const Value& val) {
		if (update_coefs(coefs, val)) {
			step(coefs, val + vec);
		}
	}

	// find first unknown variable in current @coefs
	u_i get_nearest_var(const V<V<u_i>>& coefs) {
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

		return 0;
	}

	// like dfs
	void step(const V<V<u_i>>& coefs, u_i vec = 0) {
		if (coefs.empty()) {
			// add new solution
			answers.push_back(vec);
			return;
		}

		auto index = get_nearest_var(coefs);
		// Positive
		set_value(coefs, vec, Value(index, true));
		// Negative
		set_value(coefs, vec, Value(index, false));
	}

public:
	// Note: zero coefficient means Positive value for equation
	Quine(const Solution* solution) : Method(solution) {
		if (data_ptr->linear)
			invalid_call();
	}

	V<u_i> solve() {
		answers.clear();
		step(data_ptr->coefficients);
		return answers;
	}
};

class TSS : public Method {
public:
	TSS(const Solution* solution) : Method(solution) {
		if (!data_ptr->linear)
			invalid_call();
	}
};