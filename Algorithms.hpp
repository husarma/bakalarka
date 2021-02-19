#pragma once

#include "Debug.hpp"
#include <vector>
#include <queue>
#include <thread>

void time_expanded(std::vector<std::vector<size_t>>& map_input, std::vector<std::vector<size_t>>& map_output, std::pair<std::pair<int, int>, std::pair<int, int>> agent);
void time_expanded_multithread(std::vector<std::vector<size_t>>& map_input, std::vector<std::vector<size_t>>& map_output, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>& agents);

void shortest_path(std::vector<std::vector<size_t>>& map_input, std::vector<std::vector<std::pair<size_t, size_t>>>& output_paths, size_t index_in_output, std::pair<std::pair<int, int>, std::pair<int, int>> agent);
void shortest_path_multithread(std::vector<std::vector<size_t>>& map_input, std::vector<std::vector<std::pair<size_t, size_t>>>& output_paths, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>& agents);