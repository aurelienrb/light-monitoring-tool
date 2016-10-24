#pragma once

#include <vector>
#include <string>

void updateCPUStats();

std::string getCPUJSONStats();

std::string to_json(const std::vector<int> & data);
