#pragma once
#include <chrono>


class Timer {
public:
	Timer() {

	};
	
	/// \brief Set reference time point
	void setRef() {
		start = std::chrono::steady_clock::now();
	}
	/// \brief Get time elapsed since creation in miliseconds
	unsigned long int getElapsed() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
	}
private:
	std::chrono::time_point<std::chrono::steady_clock> start;
};