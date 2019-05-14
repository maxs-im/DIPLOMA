#pragma once
#include <vector>
#include <string>
#include <bitset>
#include <set>
#include <cmath>
#include <unordered_set>

class Parsing {
protected:
	enum Actions {
		EQUAL = '=',
		ADD = '+',
		NEGATIVE = '0',
		POSITIVE = '1'
	};

	struct Error {
		enum ErrorsId {
			BIT = -1,
			NONE,
			IGNORE, INCOMPATIBILITY,
			GENERAL, NUMBER, DUPLICATE,
			EMPTY
		};
		static std::string id2str(ErrorsId id);

		std::string info;
		ErrorsId id;
		int index;
		bool is_Table;

		Error(ErrorsId _id, bool _is_Table, const std::string& _info);
		void set_index(int index);

		std::string get_error() const;
	};

	// storage for errors
	std::vector<Error> errors;

private:
	Error parse_table(const std::vector<std::string>& tokens);
	Error parse_polynom(const std::vector<std::string>& tokens);
	Error parse_line(const std::vector<std::string>& line);

public:
	// storage for variable names
	std::set<std::string> vocabulary;
	// valid equations
	std::vector<std::vector<std::string>> equations;

	Parsing(const std::vector<std::vector<std::string>>& lines);
};
