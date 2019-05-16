#pragma once
#include <vector>

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

	V<V<u_i>> solve() {
		return linear ? Quine(*this).solve() : TSS(*this).solve();
	}
};

// interface for our algorithms
class Method {
protected:
	void invalid_call() const throw(...) {
		throw "Invalid method choice. Contact to developers!";
	}
public:
	V<V<u_i>> answers; 
	virtual V<V<u_i>> solve() const {
		return answers;
	}
};

class Quine : public Method {
public:
	Quine(const Solution& solution) {
		if (solution.linear)
			invalid_call();
	}
};

class TSS : public Method {
public:
	TSS(const Solution& solution) {
		if (!solution.linear)
			invalid_call();
	}
};
