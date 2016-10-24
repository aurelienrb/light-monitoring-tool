#include "cpu-stats.h"

#include <cassert>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <iomanip>
#include <mutex>

#define _UNICODE
#include <pdh.h>
#pragma comment(lib, "Pdh.lib")

namespace {
	// http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process

	static PDH_HQUERY cpuQuery;
	static PDH_HCOUNTER cpuTotal;

	void initPdh() {
		if (PdhOpenQuery(NULL, NULL, &cpuQuery) != ERROR_SUCCESS ||
			PdhAddEnglishCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal) != ERROR_SUCCESS)
		{
			_CrtDbgBreak();
		}
		else {
			// Start outside the loop as CPU requires difference 
			PdhCollectQueryData(cpuQuery);
		}
	}

	double getCurrentCPUValue() {
		PDH_FMT_COUNTERVALUE counterVal;

		if (PdhCollectQueryData(cpuQuery) != ERROR_SUCCESS ||
			PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal) != ERROR_SUCCESS)
		{
			_CrtDbgBreak();
			return 0.0;
		}
		else {
			return counterVal.doubleValue;
		}
	}
}

static std::mutex s_statsMutex;
static std::vector<int> s_cpuStats;


void updateCPUStats() {
	std::lock_guard<std::mutex> lock(s_statsMutex);

	static bool s_initDone = false;
	if (!s_initDone) {
		initPdh();
		s_initDone = true;
	}

	const size_t maxNbValues = 300;
	if (s_cpuStats.empty()) {
		s_cpuStats.resize(maxNbValues - 1);
	}
	else if (s_cpuStats.size() == maxNbValues) {
		s_cpuStats.assign(s_cpuStats.begin() + 1, s_cpuStats.end());
	}

	const auto val = static_cast<int>(getCurrentCPUValue());
	s_cpuStats.push_back(val);
	assert(s_cpuStats.size() == maxNbValues);
}

std::string getCPUJSONStats() {
	std::lock_guard<std::mutex> lock(s_statsMutex);
	return to_json(s_cpuStats);
}

std::string to_json(const std::vector<int> & data) {
	std::ostringstream oss;
	oss << "{ "
		<< std::quoted("label") << ": " << std::quoted("CPU") << ", "
		<< std::quoted("data") << ": [";

	bool isFirst = true;
	for (size_t i = 0; i < data.size(); ++i) {
		if (i != 0) {
			oss << ", ";
		}
		oss << "[" << i+1 << ", " << data[i] << "]";
		isFirst = false;
	}

	oss << "] }";
	return oss.str();
}