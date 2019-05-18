#include "Algorithms.h"

V<V<u_i>> Solution::solve() {
	V<V<bool>> vectors;
	{
		std::shared_ptr<Solution> smart_ptr(this);
		if (linear) {
			vectors = Quine(smart_ptr).solve();
		}
		else {
			vectors = TSS(smart_ptr).solve();
		}
	}

	return {};
}