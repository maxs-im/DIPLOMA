#pragma once
#include <vector>
#include <string>
#include <bitset>
#include <set>
#include <cmath>
#include <unordered_set>

#include <iostream>

template<typename T> using V = std::vector<T>;
using S = std::string;

class Parsing {
	enum Errors {
		BIT = -1,
		NONE,
		IGNORE, INCOMPATIBILITY,
		GENERAL, NUMBER, DUPLICATE,
		EMPTY
	};

	S get_error(int code) {
		switch (code) {
		case Errors::IGNORE:
			return "0 = 0 -> can be ignored";
		case Errors::INCOMPATIBILITY:
			return "0 != 1 (incompatibility)";
		case Errors::GENERAL:
			return "Incorrect representation";
		case Errors::NUMBER:
			return "Incorrect table number of variables";
		case Errors::EMPTY:
			return "Empty equation";
		case Errors::DUPLICATE:
			return "Repeated variables (incompatibility)";
		case Errors::NONE:
			return "";
		default:
			std::string symbol(1, Errors::BIT - code);
			return "Symbol \"" + symbol + "\" is not a bit";
		}
	}

	int parse_table(const V<S>& tokens) {
		const auto& table = tokens.back();
		auto combinations_number = pow(2, tokens.size() - 1);
		// "x y 11" || "111"
		if (combinations_number != table.length()) {
			return Errors::NUMBER;
		}

		S	neg(1, Actions::NEGATIVE),
			pos(1, Actions::POSITIVE);
		if (tokens.size() == 1) {
			// "0"
			if (tokens[0] == neg)
				return Errors::IGNORE;
			// "1"
			else if (tokens[0] == pos)
				return Errors::INCOMPATIBILITY;
			// "?"
			else
				return Errors::GENERAL;
		}

		for (auto chr : table) {
			// "x y 1?00"
			if (chr != Actions::NEGATIVE && chr != Actions::POSITIVE) {
				return Errors::BIT - chr;
			}
		}

		std::unordered_set<S> buf_variables(tokens.begin(), tokens.end() - 1);
		// "x y x 01010101"
		if (buf_variables.size() != tokens.size() - 1) {
			return Errors::DUPLICATE;
		}

		// save maybe new variables
		vocabulary.insert(buf_variables.begin(), buf_variables.end());
		return Errors::NONE;
	}

	int parse_polynom(const V<S>& tokens) {
		// "="
		if (tokens.size() == 1) {
			return Errors::IGNORE;
		}

		S	add(1, Actions::ADD),
			eq(1, Actions::EQUAL),
			neg(1, Actions::NEGATIVE),
			pos(1, Actions::POSITIVE);

		std::set<S> buf_variables;
		auto buf_itr(buf_variables.begin());

		// collect variables for current polynom
		for (const auto& token : tokens) {
			if (token == add)
				continue;
			if (token == eq) {
				break;
			}
			buf_itr = buf_variables.insert(buf_itr, token);
		}

		// Note: tokens.size > 1
		auto	prev_t = tokens[tokens.size() - 2],
				last_t = tokens.back();
		// "... = ?"
		if (prev_t == eq) {
			if (last_t == pos) {
				// = 1
				if (buf_variables.size() == 0) {
					return Errors::INCOMPATIBILITY;
				}
			}
			else if (last_t == neg) {
				// = 0
				if (buf_variables.size() == 0) {
					return Errors::IGNORE;
				}
			}
			else {
				// = ???
				return Errors::BIT - last_t[0];
			}
		}
		else {
			if (last_t != eq) {
				return Errors::GENERAL;
			}
		}

		// save maybe new variables
		vocabulary.insert(buf_variables.begin(), buf_variables.end());
		return Errors::NONE;
	}

	int parse_line(const V<S>& line) {
		if (line.size() == 0) {
			return Errors::EMPTY;
		}

		S eq(1, Actions::EQUAL);
		if (std::find(line.begin(), line.end(), eq) != line.end())
			return parse_polynom(line);
		else
			return parse_table(line);
	}

protected:
	enum Actions {
		EQUAL = '=',
		ADD = '+',
		NEGATIVE = '0',
		POSITIVE = '1'
	};

public:
	// storage for variable names
	std::set<S> vocabulary;
	// valid equations
	V<V<S>> equations;

	Parsing(const V<V<S>> & lines) {
		for (int i = 0; i < lines.size(); ++i) {
			const V<S>& line = lines[i];

			if (std::find(line.begin(), line.end(), "") != line.end()) {
				// UNREACHABLE
				throw "Something goes wrong with reading. Contact to developers!\n";
			}

			auto code = parse_line(line);
			if (code == Errors::NONE) {
				equations.push_back(line);
			}
			else {
				std::cout << i + 1 << ":\t" + get_error(code) + "\n";
			}
		}
	}
};

class System_Equations : protected Parsing {
public:
	std::vector<unsigned int> coefficients;

	System_Equations(const V<V<S>>& lines) : Parsing(lines) {
		for (const auto& it : vocabulary)
			std::cout << it << "\n";
	}
};