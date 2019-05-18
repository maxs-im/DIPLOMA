#include "Algorithms.h"


V<u_i> Solution::solve() {
	V<u_i> vectors;

	if (linear) {
		vectors = TSS(this).solve();
	}
	else {
		vectors = Quine(this).solve();
	}

	return vectors;
}