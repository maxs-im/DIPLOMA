#pragma once
#include <vector>
#include <unordered_set> 
#include <utility>
#include <algorithm>

class Solution {
	static bool is_linear(const std::vector<std::vector<unsigned long long>>& _coefficients);

public:
	const std::vector<std::vector<unsigned long long>>& coefficients;
	const unsigned long long range;
	const bool linear;

	Solution(
		const std::vector<std::vector<unsigned long long>>& _coefficients,
		const unsigned long long _range
	);

	std::pair<std::vector<unsigned long long>, std::vector<unsigned long long>> solve();
};

// interface for our algorithms
class Method {
protected:
	void reduce(std::vector<unsigned long long>& equation);

	const Solution* data_ptr;
	void invalid_call() const throw(...);
public:
	Method(const Solution* _data);
	
	std::vector<unsigned long long> answers;
	std::vector<unsigned long long> basis;

	virtual std::pair<std::vector<unsigned long long>, std::vector<unsigned long long>> solve();

};

class Quine : public Method {
	struct Value {
		const unsigned long long index;
		const bool positive;

		Value(unsigned long long _index, bool _positive = true);

		int count(const unsigned long long coef) const;

		friend unsigned long long operator+ (const Value& val, const unsigned long long vec);

	private:
		unsigned long long get_number() const;
	};
	friend unsigned long long operator+ (const Value& val, const unsigned long long vec);

	bool update_coefs(
		std::vector<std::vector<unsigned long long>>& coefs, 
		const Value& val
	);

	void set_value(
		std::vector<std::vector<unsigned long long>> coefs, 
		const unsigned long long vec, 
		const Value& val
	);

	// find first unknown variable in current @coefs
	unsigned long long get_nearest_var(
		const std::vector<std::vector<unsigned long long>>& coefs
	);

	// like dfs
	void step(
		const std::vector<std::vector<unsigned long long>>& coefs, 
		unsigned long long vec = 0
	);

public:
	// Note: zero coefficient means Positive value for equation
	Quine(const Solution* solution);

	std::pair<std::vector<unsigned long long>, std::vector<unsigned long long>> solve();
};

class TSS : public Method {
	std::vector<std::vector<unsigned long long>> prepare_coefs(
		std::vector<std::vector<unsigned long long>> coefs, 
		const unsigned long long range) throw(...);

	std::vector<unsigned long long> get_start_vectors(const std::vector<unsigned long long>& first, unsigned long long range);

	bool multiply(const std::vector<unsigned long long>& equation, const unsigned long long value);

	unsigned long long calcul_sum(const std::vector<unsigned long long>& coefs, const unsigned long long vec);

	std::vector<unsigned long long> add_equation(
		const std::vector<unsigned long long>& equation, 
		const std::vector<unsigned long long>& vectors
	);

	// first -> individual, second -> basis
	std::pair<std::vector<unsigned long long>, std::vector<unsigned long long>> separate_solutions(
		const std::vector<unsigned long long>& vec
	);

public:
	TSS(const Solution* solution);

	std::pair<std::vector<unsigned long long>, std::vector<unsigned long long>> solve();
};