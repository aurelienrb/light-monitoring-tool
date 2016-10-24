#pragma once

#include <chrono>
using namespace std::chrono_literals;

namespace utils {

#ifdef _DEBUG
class noncopyable {};
#endif

class PeriodicTicker : public noncopyable {
public:
	std::chrono::milliseconds frequency() const {
		return m_triggerFrequency;
	}
	// returns the number (position) of the tick in the periodic sequence
	size_t waitNextTick();
	
private:
	std::chrono::milliseconds m_triggerFrequency = 1000ms;
	std::chrono::time_point<std::chrono::steady_clock> m_start;
};

}
