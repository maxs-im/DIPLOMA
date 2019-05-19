#include "Algorithms.h"


V<u_i> Solution::solve() {
	V<u_i> vectors;

	if (linear) {
		auto obj = TSS(this);

		vectors = obj.solve();
		auto pr = obj.separate_solutions(vectors);
		vectors = pr.first;
		vectors.insert(vectors.end(), pr.second.begin(), pr.second.end());
	}
	else {
		vectors = Quine(this).solve();
	}

	return vectors;
}