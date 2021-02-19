#pragma once

#include "Debug.hpp"
#include <vector>
#include <queue>
#include <thread>

void BFS(std::vector<std::vector<size_t>>& map_input, std::vector<std::vector<size_t>>& map_output, std::pair<std::pair<int, int>, std::pair<int, int>> agent);
void BFS_multithread(std::vector<std::vector<size_t>>& map_input, std::vector<std::vector<size_t>>& map_output, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> agents);