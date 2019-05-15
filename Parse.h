#pragma once
#include <vector>
#include <string>
#include <bitset>
#include <set>
#include <cmath>
#include <unordered_set>

class Parsing {
protected:
	const enum Actions {
		EQUAL = '=',
		ADD = '+',
		NEGATIVE = '0',
		POSITIVE = '1'
	};

	bool is_from_table(const std::vector<std::string>& line) const;

public:
	struct Error {
		const enum ErrorsId {
			BIT = -1,
			NONE,
			IGNORE, INCOMPATIBILITY,
			GENERAL, NUMBER, DUPLICATE,
			EMPTY
		};
		static std::string id2str(const ErrorsId id);

		std::string info;
		ErrorsId id;
		int index;
		bool is_Table;

		Error(const ErrorsId _id, bool _is_Table, const std::string& _info);
		void set_index(int index);

		std::string get_error() const;
	};

	// storage for variable names
	std::set<std::string> vocabulary;
	// valid equations
	std::vector<std::vector<std::string>> equations;
	// storage for errors
	std::vector<Error> errors;

	Parsing(const std::vector<std::vector<std::string>>& lines);

private:
	Error parse_table(const std::vector<std::string>& tokens);
	Error parse_polynom(const std::vector<std::string>& tokens);
	Error parse_line(const std::vector<std::string>& line);
};
