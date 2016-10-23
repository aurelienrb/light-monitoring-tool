#pragma once

#include <vector>
#include <string>

std::vector<int> getCPUStats();

std::string to_json(const std::vector<int> & data);
