#include "Algorithms.h"


#include <iostream>

V<u_i> Solution::solve() {
	V<u_i> vectors;
	std::shared_ptr<Solution> smart_ptr(this);
	if (linear) {
		vectors = TSS(smart_ptr).solve();
	}
	else {
		vectors = Quine(smart_ptr).solve();
	}

	return vectors;
}