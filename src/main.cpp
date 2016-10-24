#include "monitoring-service.h"
#include "CPUMonitor.h"
#include "HttpServer.h"

int main() {
	CPUMonitor cpuMonitor;
	MonitoringThread thread;
	thread.start([&] {
		cpuMonitor.update();
	});
	
	HttpServer server;
	server.start();
}
