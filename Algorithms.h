#pragma once
#include <vector>
#include <memory>
#include <functional>

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

	V<V<u_i>> solve();
};

// interface for our algorithms
class Method {
protected:
	const std::shared_ptr<Solution> data_ptr;
	void invalid_call() const throw(...) {
		throw "Invalid method choice. Contact to developers!";
	}
public:
	Method(const std::shared_ptr<Solution> _data) : data_ptr(_data) {};
	V<V<bool>> answers; 
	virtual V<V<bool>> solve() {
		return answers;
	}

};

class Quine : public Method {
	void reduce(V<u_i>& equation) {

	}

	struct Value {
		const u_i number;
		const bool positive;

		Value(u_i index, bool _positive = true) :
			number(1 << index),
			positive(_positive)
		{}

		u_i count(const u_i coef) const {
			if (positive) {
				auto new_val = (coef & (~number));
				// -1 - means add positive value
				return (new_val == number) ? -1 : new_val;
			}
			else {
				return ((coef & number) > 0) ? 0 : coef;
			}
		}
	};

	bool update_coefs(V<V<u_i>>& coefs, const Value& val) {
		for (int li = 0; li < coefs.size(); ++li) {
			V<u_i>& equation = coefs[li];
			for (int i = 0; i < equation.size(); ++i) {
				auto new_val = val.count(equation[i]);
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

	void set_value(const V<V<u_i>>& coefs, V<bool>& values, const Value& val) {
		auto new_coefs(coefs);
		if (update_coefs(new_coefs, val)) {
			values.push_back(val.positive);
			if (new_coefs.empty()) {
				answers.push_back(values);
			}
			else {
				step(new_coefs, values);
			}
			values.pop_back();
		}
	}

	// like dfs
	void step(const V<V<u_i>>& coefs, V<bool>& values) {
		for (u_i i = values.size(); i <= data_ptr->range; ++i) {
			auto index = data_ptr->range - i;
			set_value(coefs, values, Value(index, true));
			set_value(coefs, values, Value(index, false));
		}
	}

public:
	// Note: zero coefficient means Positive value for equation
	Quine(const std::shared_ptr<Solution>& solution) : Method(solution) {
		if (data_ptr->linear)
			invalid_call();
	}

public:
	V<V<bool>> solve() {
		answers.clear();
		V<bool> buffer;
		buffer.reserve(data_ptr->range + 1);
		step(data_ptr->coefficients, buffer);
		return answers;
	}
};

class TSS : public Method {
public:
	TSS(const std::shared_ptr<Solution>& solution) : Method(solution) {
		if (!data_ptr->linear)
			invalid_call();
	}
};