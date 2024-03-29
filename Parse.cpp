#include "Parse.h"
#include "templates.h"

Parsing::Error::Error(const ErrorsId _id, bool _is_Table, const S& _info = "") 
		: info(_info), id(_id), is_Table(_is_Table) {
	index = 0;
};
void Parsing::Error::set_index(u_i _index) {
	index = _index;
}

S Parsing::Error::id2str(const ErrorsId id) {
	switch (id) {
	case ErrorsId::IGNORE:
		return "0 = 0 -> can be ignored";
	case ErrorsId::INCOMPATIBILITY:
		return "0 != 1 -> incompatibility";
	case ErrorsId::GENERAL:
		return "Incorrect representation";
	case ErrorsId::NUMBER:
		return "Incorrect number of variables";
	case ErrorsId::EMPTY:
		return "Empty equation";
	case ErrorsId::DUPLICATE:
		return "Repeated variables -> incompatibility";
	case ErrorsId::NONE:
		return "";
	default:
		return "Symbol is not a bit";
	}
}
S Parsing::Error::get_error() const {
	auto	message = id2str(id),
			type = S(is_Table ? "Table" : "Zhegalkin"),
			prefix = type + "-> line " + std::to_string(index + 1) + ": ";
	return prefix + message + (info == "" ? "" : " (" + info + ")");
}

bool Parsing::is_from_table(const V<S>& line) const {
	S eq(1, Actions::EQUAL);
	return std::find(line.begin(), line.end(), eq) == line.end();
}

Parsing::Error Parsing::parse_table(const V<S>& tokens) {
	const auto& table = tokens.back();
	auto combinations_number = 1 << (tokens.size() - 1);
	// "x y 11" || "111"
	if (combinations_number != table.length()) {
		return Error(Error::ErrorsId::NUMBER, true);
	}

	S	neg(1, Actions::NEGATIVE),
		pos(1, Actions::POSITIVE);
	if (tokens.size() == 1) {
		// "0"
		if (tokens[0] == neg)
			return Error(Error::ErrorsId::IGNORE, true);
		// "1"
		else if (tokens[0] == pos)
			return Error(Error::ErrorsId::INCOMPATIBILITY, true);
		// "?"
		else
			return Error(Error::ErrorsId::GENERAL, true);
	}

	u_i number(0);
	// case with all same bits
	bool ignore = true;
	for (u_i i = 0; i < table.length(); ++i) {
		// "x y 1?00"
		if (
			table[i] != Actions::NEGATIVE &&
			table[i] != Actions::POSITIVE
		) {
			return Error(Error::ErrorsId::BIT, true, S(1, table[i]));
		}

		if (ignore) {
			if (table[0] != table[i]) {
				ignore = false;
			}
		}

		number <<= 1;
		if (table[i] == Actions::POSITIVE) {
			number += 1;
		}
	}

	if (ignore) {
		auto bit = S(1, table[0]);
		return Error(Error::ErrorsId::INCOMPATIBILITY, true, bit + "..." + bit + " constant function");
	}

	std::unordered_set<S> buf_variables(tokens.begin(), tokens.end() - 1);
	// "x y x 01010101"
	if (buf_variables.size() != tokens.size() - 1) {
		return Error(Error::ErrorsId::DUPLICATE, true);
	}

	// save maybe new variables
	vocabulary.insert(buf_variables.begin(), buf_variables.end());
	return Error(Error::ErrorsId::NONE, true, std::to_string(number));
}

Parsing::Error Parsing::parse_polynom(const V<S> & tokens) {
	// "="
	if (tokens.size() == 1) {
		return Error(Error::ErrorsId::IGNORE, false);
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
				return Error(Error::ErrorsId::INCOMPATIBILITY, false);
			}
		}
		else if (last_t == neg) {
			// = 0
			if (buf_variables.size() == 0) {
				return Error(Error::ErrorsId::IGNORE, false);
			}
		}
		else {
			// = ???
			return Error(Error::ErrorsId::BIT, false, last_t);
		}
	}
	else {
		if (last_t != eq) {
			return Error(Error::ErrorsId::GENERAL, false);
		}
	}

	// save maybe new variables
	vocabulary.insert(buf_variables.begin(), buf_variables.end());
	return Error(Error::ErrorsId::NONE, false);
}

Parsing::Error Parsing::parse_line(const V<S> & line) {
	if (line.size() == 0) {
		return Error(Error::ErrorsId::EMPTY, false);
	}

	if (is_from_table(line))
		return parse_table(line);
	else
		return parse_polynom(line);
}

Parsing::Parsing(const V<V<S>>& lines) {
	for (u_i i = 0; i < lines.size(); ++i) {
		V<S> line = lines[i];

		if (std::find(line.begin(), line.end(), "") != line.end()) {
			// UNREACHABLE
			throw "Something goes wrong with reading. Contact to developers!";
		}

		auto error = parse_line(line);
		if (error.id == Error::ErrorsId::NONE) {
			if (error.is_Table) {
				line[line.size() - 1] = error.info;
			}
			equations.push_back(line);
		}
		else {
			// update and save new error
			error.set_index(i);
			errors.push_back(error);
		}
	}
}
