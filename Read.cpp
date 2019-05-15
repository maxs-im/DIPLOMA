#include "Read.h"

template<typename T> using V = std::vector<T>;
using S = std::string;
using VS = V<S>;
using VVS = V<V<S>>;

using namespace Read;

VS split_string(const S& str) {
	std::istringstream buf(str);
	std::istream_iterator<S> beg(buf), end;

	return VS(beg, end);
}

VVS Read::read_file(std::istream& stream) throw(...) {
	VVS strlines(0);
	S str;
	if (!stream) {
		throw "Bad stream (check file)";
	}

	while (stream) {
		getline(stream, str);
		if (str == "")
			break;
		strlines.push_back(split_string(str));
	}

	return strlines;
}
