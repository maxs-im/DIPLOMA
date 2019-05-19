#include "Timer.h"

Timer::Timer() : time(clock_t::now()) {};

void Timer::reset() {
	time = clock_t::now();
}

double Timer::elapsed() const {
	return std::chrono::duration_cast<second_t>(clock_t::now() - time).count();
}