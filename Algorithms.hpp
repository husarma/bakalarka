#pragma once

#include "Debug.hpp"
#include <vector>
#include <queue>
#include <thread>

void time_expanded(std::vector<std::vector<size_t>>& referenece_map, std::vector<std::vector<size_t>>& map_output, std::pair<std::pair<int, int>, std::pair<int, int>> agent);
std::string time_expanded_multiagent(std::vector<std::vector<size_t>>& referenece_map, std::vector<std::vector<size_t>>& map_output, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>& agents);

void shortest_path(std::vector<std::vector<size_t>>& referenece_map, std::vector<std::vector<std::pair<size_t, size_t>>>& output_paths, size_t index_in_output, std::pair<std::pair<int, int>, std::pair<int, int>> agent);
std::string shortest_path_multiagent(std::vector<std::vector<size_t>>& referenece_map, std::vector<std::vector<std::pair<size_t, size_t>>>& output_paths, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>& agents);

void paths_to_map(std::vector<std::vector<std::pair<size_t, size_t>>>& input_paths, std::vector<std::vector<size_t>>& map_output);

std::string map_union(std::vector<std::vector<size_t>>& map1, std::vector<std::vector<size_t>>& map2, std::vector<std::vector<size_t>>& map_output);
std::string map_intersection(std::vector<std::vector<size_t>>& map1, std::vector<std::vector<size_t>>& map2, std::vector<std::vector<size_t>>& map_output);

std::string expand_map(std::vector<std::vector<size_t>>& reference_map, std::vector<std::vector<size_t>>& map_to_surround, std::vector<std::vector<size_t>>& map_output);

bool are_paths_separate(std::vector<std::vector<std::pair<size_t, size_t>>>& input_paths);