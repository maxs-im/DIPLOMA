#pragma once
#include <chrono>

class Timer {
private:
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1>>;

	std::chrono::time_point<clock_t> time;

public:
	Timer();

	void reset();

	double elapsed() const;
};
