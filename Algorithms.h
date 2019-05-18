#pragma once
#include <vector>
#include <memory>

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
	V<V<bool>> step(V<V<u_i>> coefs, V<bool>& values) {
		for (u_i i = values.size(); i <= data_ptr->range; ++i) {
			
		}

		return {};
	}

public:
	Quine(const std::shared_ptr<Solution>& solution) : Method(solution) {
		if (data_ptr->linear)
			invalid_call();
	}

public:
	V<V<bool>> solve() {
		V<bool> buffer;
		buffer.reserve(data_ptr->range + 1);
		return step(data_ptr->coefficients, buffer);
	}
};

class TSS : public Method {
public:
	TSS(const std::shared_ptr<Solution>& solution) : Method(solution) {
		if (!data_ptr->linear)
			invalid_call();
	}
};