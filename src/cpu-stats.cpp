#include "cpu-stats.h"

#include <sstream>
#include <cstdlib>

std::vector<int> getCPUStats() {
	static std::vector<int> stats;
	const size_t nbValues = 60;
	if (!stats.empty()) {
		stats.assign(stats.begin() + 1, stats.end());
	}
	while (stats.size() < nbValues) {
		const int n = std::rand() % 10;
		stats.push_back(n);
	}
	return stats;
}

//const std::string result = "{ \"cpu\": [10, 15, 27, 17, 54, 68, 77, 61, 89, 47, 54, 63, 57, 65, 74, 88, 97, 95, 91, 98, 92, 84, 75, 31, 12, 5, 9, 5, 11, 4, 6, 8, 4] }";

std::string to_json(const std::vector<int> & data) {
	std::ostringstream oss;
	oss << "{ \"cpu\": [";
	bool isFirst = true;
	for (int n : data) {
		oss << (isFirst ? "" : ", ") << n;
		isFirst = false;
	}
	oss << "] }";
	return oss.str();
}