#include "monitoring-service.h"
#include "PeriodicTicker.h"
#include <cassert>

MonitoringThread::~MonitoringThread() {
	stop();
}

void MonitoringThread::start(std::function<void()> callback) {
	assert(!m_thread);

	auto threadProc = [this, callback = std::move(callback)] {
		utils::PeriodicTicker ticker;
		while (m_keepRunning) {
			const size_t tickNumber = ticker.waitNextTick();
			callback();
			//std::cout << ticker.frequency().count() * tickNumber / 1000.0 << " sec\n";
		}
	};

	std::lock_guard<std::mutex> lock(m_mutex);
	if (!m_thread) {
		m_keepRunning = true;
		m_thread.reset(new std::thread(threadProc));
	}
}

void MonitoringThread::stop() {
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_thread) {
		m_keepRunning = false;
		m_thread->join();
		m_thread.reset();
	}
}

