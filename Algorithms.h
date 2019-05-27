#pragma once
#include <vector>
#include <unordered_set> 
#include <utility>
#include <algorithm>

class Solution {
	static bool is_linear(const std::vector<std::vector<size_t>>& _coefficients);

public:
	const std::vector<std::vector<size_t>>& coefficients;
	const size_t range;
	const bool linear;

	Solution(
		const std::vector<std::vector<size_t>>& _coefficients,
		const size_t _range
	);

	std::pair<std::vector<size_t>, std::vector<size_t>> solve(bool prefer_Quine = false);
};

// interface for our algorithms
class Method {
protected:
	void reduce(std::vector<size_t>& equation);

	const Solution* data_ptr;
	void invalid_call() const throw(...);
public:
	Method(const Solution* _data);
	
	std::vector<size_t> answers;
	std::vector<size_t> basis;

	virtual std::pair<std::vector<size_t>, std::vector<size_t>> solve();

};

class Quine : public Method {
	struct Value {
		const size_t index;
		const bool positive;

		Value(size_t _index, bool _positive = true);

		size_t count(const size_t coef) const throw (...);

		friend size_t operator+ (const Value& val, const size_t vec);

	private:
		size_t get_number() const;
	};
	friend size_t operator+ (const Value& val, const size_t vec);

	bool update_coefs(
		std::vector<std::vector<size_t>>& coefs, 
		const Value& val
	);

	void set_value(
		std::vector<std::vector<size_t>> coefs, 
		const size_t vec, 
		const Value& val
	);

	// find first unknown variable in current @coefs
	size_t get_nearest_var(
		const std::vector<std::vector<size_t>>& coefs
	) throw (...);

	// like dfs
	void step(
		const std::vector<std::vector<size_t>>& coefs, 
		size_t vec = 0
	);

public:
	// Note: zero coefficient means Positive value for equation
	Quine(const Solution* solution);

	std::pair<std::vector<size_t>, std::vector<size_t>> solve();
};

class TSS : public Method {
	std::vector<std::vector<size_t>> prepare_coefs(
		std::vector<std::vector<size_t>> coefs, 
		const size_t range) throw(...);

	std::vector<size_t> get_start_vectors(const std::vector<size_t>& first, size_t range);

	bool multiply(const std::vector<size_t>& equation, const size_t value);

	size_t calcul_sum(const std::vector<size_t>& coefs, const size_t vec);

	std::vector<size_t> add_equation(
		const std::vector<size_t>& equation, 
		const std::vector<size_t>& vectors
	);

	// first -> individual, second -> basis
	std::pair<std::vector<size_t>, std::vector<size_t>> separate_solutions(
		const std::vector<size_t>& vec
	);

public:
	TSS(const Solution* solution);

	std::pair<std::vector<size_t>, std::vector<size_t>> solve();
};