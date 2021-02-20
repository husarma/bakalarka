#include "Algorithms.hpp"

/** Computes time expanded graph for one agent.
* 
* Uses BFS algorithm from start to finish and from finish to start and as a results 
* takes intersection of used vertices.
* 
* @param reference_map original input map.
* @param map_output map for result writing.
* @param agent pair containig agents start and finish coordinates in pair.
*/
void time_expanded(std::vector<std::vector<size_t>>& reference_map, std::vector<std::vector<size_t>>& map_output, std::pair<std::pair<int, int>, std::pair<int, int>> agent) {

	std::vector<std::vector<size_t>> temp_map(reference_map.size(), std::vector<size_t>(reference_map[0].size(), 0));

	std::queue<std::pair<size_t, size_t>> vertex_queue;

	bool found = false;
	vertex_queue.push(agent.first);

	while (!(found || vertex_queue.empty())) {

		//push lap delimiter
		vertex_queue.push(std::make_pair(0, 0));

		while (vertex_queue.front().first != 0) {
			std::pair<size_t, size_t> a = vertex_queue.front();
			vertex_queue.pop();

			if (a.first == agent.second.first && a.second == agent.second.second) {
				found = true;
			}

			if (reference_map[a.first - 1][a.second] != 0 && temp_map[a.first - 1][a.second] != 2) {
				temp_map[a.first - 1][a.second] = 2;
				vertex_queue.push(std::make_pair(a.first - 1, a.second));
			}
			if (reference_map[a.first][a.second + 1] != 0 && temp_map[a.first][a.second + 1] != 2) {
				temp_map[a.first][a.second + 1] = 2;
				vertex_queue.push(std::make_pair(a.first, a.second + 1));
			}
			if (reference_map[a.first + 1][a.second] != 0 && temp_map[a.first + 1][a.second] != 2) {
				temp_map[a.first + 1][a.second] = 2;
				vertex_queue.push(std::make_pair(a.first + 1, a.second));
			}
			if (reference_map[a.first][a.second - 1] != 0 && temp_map[a.first][a.second - 1] != 2) {
				temp_map[a.first][a.second - 1] = 2;
				vertex_queue.push(std::make_pair(a.first, a.second - 1));
			}
		}

		//pop lap delimiter
		vertex_queue.pop();
	}


	found = false;
	vertex_queue = std::queue<std::pair<size_t, size_t>>();
	vertex_queue.push(agent.second);

	while (!(found || vertex_queue.empty())) {

		//push lap delimiter
		vertex_queue.push(std::make_pair(0, 0));

		while (vertex_queue.front().first != 0) {
			std::pair<size_t, size_t> a = vertex_queue.front();
			vertex_queue.pop();

			if (a.first == agent.first.first && a.second == agent.first.second) {
				found = true;
			}
			
			if (temp_map[a.first][a.second] == 3) {
				map_output[a.first][a.second] = 1;
			}

			if (reference_map[a.first - 1][a.second] != 0 && temp_map[a.first - 1][a.second] % 2 == 0) {
				if (temp_map[a.first - 1][a.second] % 2 == 0) {
					temp_map[a.first - 1][a.second] += 1;
				}
				vertex_queue.push(std::make_pair(a.first - 1, a.second));
			}
			if (reference_map[a.first][a.second + 1] != 0 && temp_map[a.first][a.second + 1] % 2 == 0) {
				if (temp_map[a.first][a.second + 1] % 2 == 0) {
					temp_map[a.first][a.second + 1] += 1;
				}
				vertex_queue.push(std::make_pair(a.first, a.second + 1));
			}
			if (reference_map[a.first + 1][a.second] != 0 && temp_map[a.first + 1][a.second] % 2 == 0) {
				if (temp_map[a.first + 1][a.second] % 2 == 0) {
					temp_map[a.first + 1][a.second] += 1;
				}
				vertex_queue.push(std::make_pair(a.first + 1, a.second));
			}
			if (reference_map[a.first][a.second - 1] != 0 && temp_map[a.first][a.second - 1] % 2 == 0) {
				if (temp_map[a.first][a.second - 1] % 2 == 0) {
					temp_map[a.first][a.second - 1] += 1;
				}
				vertex_queue.push(std::make_pair(a.first, a.second - 1));
			}
		}

		//pop lap delimiter
		vertex_queue.pop();
	}
}

/** Computes time expanded graph for group of agents.
* 
* Multithreading is used for computing.
*
* @param reference_map original input map.
* @param map_output map for result writing.
* @param agents vector of pairs containig agents start and finish coordinates in pair.
* @return error message, "OK" if everything ended well.
*/
std::string time_expanded_multiagent(std::vector<std::vector<size_t>>& reference_map, std::vector<std::vector<size_t>>& map_output, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>& agents) {
	
	if (reference_map.size() != map_output.size() || reference_map[0].size() != map_output[0].size()) {
		return "ERROR: different lenghts of maps in union\n";
	}

	std::vector<std::thread> threads;

	//Launch threads
	for (size_t i = 0; i < agents.size(); i++) {
		threads.push_back(std::thread(time_expanded, std::ref(reference_map), std::ref(map_output), agents[i]));
	}

	//Join the threads with the main thread
	for (auto& thread : threads) {
		thread.join();
	}

	return "OK";
}

/** Computes the shortest path in graph for one agent.
* 
* Uses BFS algorithm.
*
* @param reference_map original input map.
* @param output_paths vector of pairs for writing the result for agent.
* @param index_in_output index in output_paths to write result.
* @param agent vector of pairs containig agents start and finish coordinates in pair.
*/
void shortest_path(std::vector<std::vector<size_t>>& reference_map, std::vector<std::vector<std::pair<size_t, size_t>>>& output_paths, size_t index_in_output, std::pair<std::pair<int, int>, std::pair<int, int>> agent) {
	
	std::vector<std::vector<std::pair<size_t, size_t>>> temp_map(reference_map.size(), std::vector<std::pair<size_t, size_t>>(reference_map[0].size(), std::make_pair(0, 0)));

	std::queue<std::pair<size_t, size_t>> vertex_queue;

	bool found = false;
	vertex_queue.push(agent.second);

	while (!vertex_queue.empty() && !found) {
		std::pair<size_t, size_t> a = vertex_queue.front();
		vertex_queue.pop();

		//going from finish to start for better reproduction of the path
		if (a.first == agent.first.first && a.second == agent.first.second) {
			found = true;
		}

		if (reference_map[a.first - 1][a.second] != 0 && temp_map[a.first - 1][a.second].first == 0) {
			temp_map[a.first - 1][a.second].first = a.first;
			temp_map[a.first - 1][a.second].second = a.second;
			vertex_queue.push(std::make_pair(a.first - 1, a.second));
		}
		if (reference_map[a.first][a.second + 1] != 0 && temp_map[a.first][a.second + 1].first == 0) {
			temp_map[a.first][a.second + 1].first = a.first;
			temp_map[a.first][a.second + 1].second = a.second;
			vertex_queue.push(std::make_pair(a.first, a.second + 1));
		}
		if (reference_map[a.first + 1][a.second] != 0 && temp_map[a.first + 1][a.second].first == 0) {
			temp_map[a.first + 1][a.second].first = a.first;
			temp_map[a.first + 1][a.second].second = a.second;
			vertex_queue.push(std::make_pair(a.first + 1, a.second));
		}
		if (reference_map[a.first][a.second - 1] != 0 && temp_map[a.first][a.second - 1].first == 0) {
			temp_map[a.first][a.second - 1].first = a.first;
			temp_map[a.first][a.second - 1].second = a.second;
			vertex_queue.push(std::make_pair(a.first, a.second - 1));
		}
	}

	//reproducting the path from start to finish
	if (found) {
		std::pair<size_t, size_t> current = agent.first;
		output_paths[index_in_output].push_back(std::make_pair(agent.first.first, agent.first.second));
		while (current.first != agent.second.first || current.second != agent.second.second) {
			current = temp_map[current.first][current.second];
			output_paths[index_in_output].push_back(std::make_pair(current.first, current.second));
		}
	}
}

/** Computes the shortest path in graph for group of agents.
*
* Multithreading is used for computing.
*
* @param reference_map original input map.
* @param output_paths vector of pairs for writing the result for agent.
* @param agents vector of pairs containig agents start and finish coordinates in pair.
* @return error message, "OK" if everything ended well.
*/
std::string shortest_path_multiagent(std::vector<std::vector<size_t>>& reference_map, std::vector<std::vector<std::pair<size_t, size_t>>>& output_paths, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>& agents) {

	if (output_paths.size() != agents.size()) {
		return "ERROR: different lenghts of paths and agents\n";
	}

	std::vector<std::thread> threads;

	//Launch threads
	for (size_t i = 0; i < agents.size(); i++) {
		threads.push_back(std::thread(shortest_path, std::ref(reference_map), std::ref(output_paths), i, agents[i]));
	}

	//Join the threads with the main thread
	for (auto& thread : threads) {
		thread.join();
	}

	return "OK";
}

/** Writes paths to map.
*
* @param input_paths vector of pairs including agents paths.
* @param map_output map for result writing.
*/
void paths_to_map(std::vector<std::vector<std::pair<size_t, size_t>>>& input_paths, std::vector<std::vector<size_t>>& map_output) {

	for (size_t i = 0; i < input_paths.size(); i++) {
		for (size_t j = 0; j < input_paths[i].size(); j++) {
			map_output[input_paths[i][j].first][input_paths[i][j].second] = 1;
		}
	}
}

/** Makes union of free vertices from two maps.
*
* @param map1 first map for union.
* @param map2 second map for union.
* @param map_output map for result writing, can be one of unified maps.
*/
std::string map_union(std::vector<std::vector<size_t>>& map1, std::vector<std::vector<size_t>>& map2, std::vector<std::vector<size_t>>& map_output) {

	if (map1.size() != map2.size() || map1[0].size() != map2[0].size()) {
		return "ERROR: different lenghts of maps in union\n";
	}

	for (size_t i = 0; i < map1.size(); i++) {
		for (size_t j = 0; j < map1[0].size(); j++) {
			map_output[i][j] = map1[i][j] | map2[i][j];
		}
	}

	return "OK";
}

/** Makes intersection of free vertices from two maps.
*
* @param map1 first map for intersect.
* @param map2 second map for intersect.
* @param map_output map for result writing, can be one of intersected maps.
*/
std::string map_intersection(std::vector<std::vector<size_t>>& map1, std::vector<std::vector<size_t>>& map2, std::vector<std::vector<size_t>>& map_output) {

	if (map1.size() != map2.size() || map1[0].size() != map2[0].size()) {
		return "ERROR: different lenghts of maps in union\n";
	}

	for (size_t i = 0; i < map1.size(); i++) {
		for (size_t j = 0; j < map1[0].size(); j++) {
			map_output[i][j] = map1[i][j] & map2[i][j];
		}
	}

	return "OK";
}

/** Expand free vertices in map by souranding the existing ones.
*
* Can write the result back to the same map, but it is slower, 
* because it need to copy the map for computing
*
* @param reference_map original input map.
* @param map_to_expand map to be expanded.
* @param map_output map for result writing, can be the same as expanded map.
* @return error message, "OK" if everything ended well.
*/
std::string expand_map(std::vector<std::vector<size_t>>& reference_map, std::vector<std::vector<size_t>>& map_to_expand, std::vector<std::vector<size_t>>& map_output) {
	
	if (reference_map.size() != map_output.size() || reference_map[0].size() != map_output[0].size() ||
		map_to_expand.size() != map_output.size() || map_to_expand[0].size() != map_output[0].size() ||
		reference_map.size() != map_to_expand.size() || reference_map[0].size() != map_to_expand[0].size()) {
		return "ERROR: different lenghts of maps in surroundings\n";
	}

	bool same = false;
	std::vector<std::vector<size_t>> copy;
	if (&map_to_expand == &map_output) {
		same = true;
		copy = map_to_expand;
	}

	bool surround = false;
	for (size_t i = 0; i < reference_map.size(); i++) {
		for (size_t j = 0; j < reference_map[0].size(); j++) {
			
			if (same) {
				if (copy[i][j] != 0) {
					surround = true;
				}
			}
			else if (map_to_expand[i][j] != 0) {
				surround = true;
			}

			if (surround) {
				if (reference_map[i - 1][j] != 0) {
					map_output[i - 1][j] = 1;
				}
				if (reference_map[i][j + 1] != 0) {
					map_output[i][j + 1] = 1;
				}
				if (reference_map[i + 1][j] != 0) {
					map_output[i + 1][j] = 1;
				}
				if (reference_map[i][j - 1] != 0) {
					map_output[i][j - 1] = 1;
				}

				surround = false;
			}
		}
	}

	return "OK";
}

/** Writes paths to map.
*
* @param input_paths vector of pairs including agents paths.
* @return true if paths have something common, false elsewhere
*/
bool are_paths_separate(std::vector<std::vector<std::pair<size_t, size_t>>>& input_paths) {

	for (size_t curr_path = 0; curr_path < input_paths.size(); curr_path++) {
		for (size_t curr_position = 0; curr_position < input_paths[curr_path].size(); curr_position++) {

			for (size_t searched_path = curr_path + 1; searched_path < input_paths.size(); searched_path++) {
				for (size_t searched_position = 0; searched_position < input_paths[searched_path].size(); searched_position++) {

					if (input_paths[curr_path][curr_position] == input_paths[searched_path][searched_position]) {
						return false;
					}
				}
			}
		}
	}
	return true;
}