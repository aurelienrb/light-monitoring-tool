#include "cpu-stats.h"

#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <iomanip>

std::vector<int> getCPUStats() {
	static std::vector<int> stats;
	const size_t nbValues = 300;
	if (!stats.empty()) {
		stats.assign(stats.begin() + 1, stats.end());
		stats.resize(stats.size() - 1);
	}
	while (stats.size() < nbValues) {
		const int n = (std::rand() % 100);
		stats.push_back(n);
	}
	return stats;
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