#include "Parse.h";

template<typename T> using V = std::vector<T>;
using S = std::string;

Parsing::Error::Error(const ErrorsId _id, bool _is_Table, const S& _info = "") 
		: info(_info), id(_id), is_Table(_is_Table) {
	index = -1;
};
void Parsing::Error::set_index(int _index) {
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
			type = S(is_Table ? "Table" : "Full"),
			prefix = type + " " + std::to_string(index) + ": ";
	return prefix + message + (info == "" ? "" : " (" + info + ")");
}

bool Parsing::is_from_table(const V<S>& line) const {
	S eq(1, Actions::EQUAL);
	return std::find(line.begin(), line.end(), eq) == line.end();
}

Parsing::Error Parsing::parse_table(const V<S>& tokens) {
	const auto& table = tokens.back();
	auto combinations_number = pow(2, tokens.size() - 1);
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

	// case with all null x y 0000  <=>  0 = 0
	bool ignore = true;
	for (int i = 0; i < table.length(); ++i) {
		// "x y 1?00"
		switch (table[i])
		{
		case Actions::NEGATIVE:
			continue;
		case Actions::POSITIVE:
			if (ignore && (i == table.length() - 1)) {
				// x 01  <=>  1 = 0
				return Error(Error::ErrorsId::INCOMPATIBILITY, true, "0...01 case without variables");
			}
			else {
				ignore = false;
			}
			continue;
		default:
			return Error(Error::ErrorsId::BIT, true, S(1, table[i]));
		}
	}

	if (ignore) {
		return Error(Error::ErrorsId::IGNORE, true, "0..0 case without variables");
	}

	std::unordered_set<S> buf_variables(tokens.begin(), tokens.end() - 1);
	// "x y x 01010101"
	if (buf_variables.size() != tokens.size() - 1) {
		return Error(Error::ErrorsId::DUPLICATE, true);
	}

	// save maybe new variables
	vocabulary.insert(buf_variables.begin(), buf_variables.end());
	return Error(Error::ErrorsId::NONE, true);
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
	for (size_t i = 0; i < lines.size(); ++i) {
		const V<S>& line = lines[i];

		if (std::find(line.begin(), line.end(), "") != line.end()) {
			// UNREACHABLE
			throw "Something goes wrong with reading. Contact to developers!\n";
		}

		auto error = parse_line(line);
		if (error.id == Error::ErrorsId::NONE) {
			equations.push_back(line);
		}
		else {
			// update and save new error
			error.set_index(i);
			errors.push_back(error);
		}
	}
}
