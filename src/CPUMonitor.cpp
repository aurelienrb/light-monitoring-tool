#include "CPUMonitor.h"
#include "cpu-stats.h"

void CPUMonitor::update() {
	updateCPUStats();
}