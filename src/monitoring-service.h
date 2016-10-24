#pragma once

#include <functional>
#include <thread>
#include <memory>
#include <atomic>
#include <mutex>

#ifdef _DEBUG
class noncopyable {};
#endif

class MonitoringThread : public noncopyable {
public:
	MonitoringThread() = default;
	~MonitoringThread();

	void start(std::function<void()> callback);
	void stop();

private:
	std::mutex m_mutex;
	std::unique_ptr<std::thread> m_thread;
	std::atomic<bool> m_keepRunning{ false };
};
