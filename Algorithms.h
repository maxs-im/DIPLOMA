#pragma once
#include <vector>
#include <unordered_set> 
#include <utility>
#include <algorithm>

class Solution {
	static bool is_linear(const std::vector<std::vector<unsigned int>>& _coefficients);

public:
	const std::vector<std::vector<unsigned int>>& coefficients;
	const unsigned int range;
	const bool linear;

	Solution(
		const std::vector<std::vector<unsigned int>>& _coefficients,
		const unsigned int _range
	);

	std::vector<unsigned int> solve();
};

// interface for our algorithms
class Method {
protected:
	void reduce(std::vector<unsigned int>& equation);

	const Solution* data_ptr;
	void invalid_call() const throw(...);
public:
	Method(const Solution* _data);
	std::vector<unsigned int> answers;
	virtual std::vector<unsigned int> solve();

};

class Quine : public Method {
	struct Value {
		const unsigned int index;
		const bool positive;

		Value(unsigned int _index, bool _positive = true);

		int count(const unsigned int coef) const;

		friend unsigned int operator+ (const Value& val, const unsigned int vec);

	private:
		unsigned int get_number() const;
	};
	friend unsigned int operator+ (const Value& val, const unsigned int vec);

	bool update_coefs(
		std::vector<std::vector<unsigned int>>& coefs, 
		const Value& val
	);

	void set_value(
		std::vector<std::vector<unsigned int>> coefs, 
		const unsigned int vec, 
		const Value& val
	);

	// find first unknown variable in current @coefs
	unsigned int get_nearest_var(
		const std::vector<std::vector<unsigned int>>& coefs
	);

	// like dfs
	void step(
		const std::vector<std::vector<unsigned int>>& coefs, 
		unsigned int vec = 0
	);

public:
	// Note: zero coefficient means Positive value for equation
	Quine(const Solution* solution);

	std::vector<unsigned int> solve();
};

class TSS : public Method {
	std::vector<std::vector<unsigned int>> prepare_coefs(
		std::vector<std::vector<unsigned int>> coefs, 
		const unsigned int range) throw(...);

	std::vector<unsigned int> get_start_vectors(const std::vector<unsigned int>& first, unsigned int range);

	bool multiply(const std::vector<unsigned int>& equation, const unsigned int value);

	unsigned int calcul_sum(const std::vector<unsigned int>& coefs, const unsigned int vec);

	std::vector<unsigned int> add_equation(
		const std::vector<unsigned int>& equation, 
		const std::vector<unsigned int>& vectors
	);

public:
	TSS(const Solution* solution);

	// first -> individual, second -> basis
	std::pair<std::vector<unsigned int>, std::vector<unsigned int>> separate_solutions(
		const std::vector<unsigned int>& vec
	);

	std::vector<unsigned int> solve();
};