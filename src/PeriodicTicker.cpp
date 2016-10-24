#include "PeriodicTicker.h"

#include <thread>

using namespace std::chrono;

namespace utils {

size_t PeriodicTicker::waitNextTick() {
	if (m_start.time_since_epoch().count() == 0) {
		m_start = std::chrono::high_resolution_clock::now();
	}

	const duration<double, std::milli> timeToWait = m_triggerFrequency - ((high_resolution_clock::now() - m_start) % m_triggerFrequency);
	std::this_thread::sleep_for(timeToWait);

	const size_t tickNumber = duration_cast<milliseconds>(high_resolution_clock::now() - m_start) / m_triggerFrequency;
	return tickNumber;
}

}
