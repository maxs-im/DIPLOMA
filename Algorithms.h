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
		auto vectors = linear ? Quine(*this).solve() : TSS(*this).solve();
		return {};
	}
};

// interface for our algorithms
class Method {
protected:
	const Solution data;
	void invalid_call() const throw(...) {
		throw "Invalid method choice. Contact to developers!";
	}
public:
	Method(const Solution& _data) : data(_data) {};
	V<V<bool>> answers; 
	virtual V<V<bool>> solve() const {
		return answers;
	}

};

class Quine : public Method {
	V<V<bool>> step(V<V<u_i>> coefs, V<bool>& values) {
		for (u_i i = values.size(); i <= data.range; ++i) {
			
		}
		return {};
	}

public:
	Quine(const Solution& solution) : Method(solution) {
		if (data.linear)
			invalid_call();
	}

	V<V<bool>> solve() {
		V<bool> buffer;
		buffer.reserve(data.range + 1);
		return step(data.coefficients, buffer);
	}
};

class TSS : public Method {
public:
	TSS(const Solution& solution) : Method(solution) {
		if (!data.linear)
			invalid_call();
	}
};