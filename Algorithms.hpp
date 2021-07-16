#pragma once

#include "Debug.hpp"
#include <vector>
#include <queue>
#include <thread>

void bubble(std::vector<std::vector<size_t>>& referenece_map, std::vector<std::vector<size_t>>& map_output, std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>> agent);
std::string bubble_multiagent(std::vector<std::vector<size_t>>& referenece_map, std::vector<std::vector<size_t>>& map_output, std::vector<std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>>>& agents);

void shortest_path(std::vector<std::vector<size_t>>& referenece_map, std::vector<std::vector<std::pair<size_t, size_t>>>& output_paths, size_t index_in_output, std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>> agent);
std::string shortest_path_multiagent(std::vector<std::vector<size_t>>& referenece_map, std::vector<std::vector<std::pair<size_t, size_t>>>& output_paths, std::vector<std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>>>& agents);

void paths_to_map(std::vector<std::vector<std::pair<size_t, size_t>>>& input_paths, std::vector<std::vector<size_t>>& map_output);

std::string map_union(std::vector<std::vector<size_t>>& map1, std::vector<std::vector<size_t>>& map2, std::vector<std::vector<size_t>>& map_output);
std::string map_intersection(std::vector<std::vector<size_t>>& map1, std::vector<std::vector<size_t>>& map2, std::vector<std::vector<size_t>>& map_output);

std::pair<std::string, bool> expand_map(std::vector<std::vector<size_t>>& reference_map, std::vector<std::vector<size_t>>& map_to_surround, std::vector<std::vector<size_t>>& map_output);

bool are_paths_separate(std::vector<std::vector<std::pair<size_t, size_t>>>& input_paths);

size_t give_new_numbering(std::vector<std::vector<size_t>>& map_to_renumber);

void time_expanded(std::vector<std::vector<size_t>>& input_map, std::vector<std::vector<std::vector<size_t>>>& output_time_expanded_draph, size_t index_in_output, std::pair<size_t, size_t> agent, size_t time);
std::string time_expanded_multiagent(std::vector<std::vector<size_t>>& input_map, std::pair<std::vector<std::vector<std::vector<size_t>>>, std::vector<std::vector<std::vector<size_t>>>>& output_time_expanded_draph, std::vector<std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>>>& agents, size_t time);

std::string generate_agents_for_map(std::vector<std::vector<size_t>>& input_map, std::string map_name, std::string output_dir, size_t number_of_instances, size_t agents_in_instance);