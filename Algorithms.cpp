#include "Algorithms.hpp"

#include <algorithm>

/** Computes bubble for one agent.
* 
* Uses BFS algorithm from start to finish and from finish to start and as a results 
* takes intersection of used vertices.
* 
* @param reference_map original input map.
* @param map_output map for result writing.
* @param agent pair containig agents start and finish coordinates in pair.
*/
void bubble(std::vector<std::vector<size_t>>& reference_map, std::vector<std::vector<size_t>>& map_output, std::pair<std::pair<int, int>, std::pair<int, int>> agent) {

	std::vector<std::vector<size_t>> temp_map(reference_map.size(), std::vector<size_t>(reference_map[0].size(), 0));

	std::queue<std::pair<size_t, size_t>> vertex_queue;

	bool found = false;
	vertex_queue.push(agent.first);
	temp_map[agent.first.first][agent.first.second] = 2;

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
	temp_map[agent.second.first][agent.second.second] += 1;

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

/** Computes bubbles for group of agents.
* 
* Multithreading is used for computing.
*
* @param reference_map original input map.
* @param map_output map for result writing.
* @param agents vector of pairs containig agents start and finish coordinates in pair.
* @return error message, "OK" if everything ended right.
*/
std::string bubble_multiagent(std::vector<std::vector<size_t>>& reference_map, std::vector<std::vector<size_t>>& map_output, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>& agents) {
	
	if (reference_map.size() != map_output.size() || reference_map[0].size() != map_output[0].size()) {
		return "ERROR: different lenghts of maps in union\n";
	}

	std::vector<std::thread> threads;

	//Launch threads
	for (size_t i = 0; i < agents.size(); i++) {
		threads.push_back(std::thread(bubble, std::ref(reference_map), std::ref(map_output), agents[i]));
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
* @param agent pair containig agent's start and finish coordinates in pair.
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
* @param output_paths vector for writing the result for agents.
* @param agents vector of pairs containig agents start and finish coordinates in pair.
* @return error message, "OK" if everything ended right.
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
* @return error message, "OK" if everything ended right.
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

/** Gives new appropriete number to each vertex.
*
* @param map_to_renumber map to be renumbered.
*/
void give_new_numbering(std::vector<std::vector<size_t>>& map_to_renumber) {

	size_t vertex_number = 1;
	for (size_t i = 0; i < map_to_renumber.size(); i++) {
		for (size_t j = 0; j < map_to_renumber[0].size(); j++) {
			if (map_to_renumber[i][j] != 0) {
				map_to_renumber[i][j] = vertex_number;
				vertex_number++;
			}
		}
	}
}

/** Computes time expanded graph for one agent.
*
* Uses BFS algorithm.
*
* @param input_map must be correctly numbered.
* @param output_time_expanded_draph vector for writing the result for agent.
* @param index_in_output index in output_time_expanded_draph to write result.
* @param agent pair containig agent's start.
* @param time makespan for time expanded graph.
*/
void time_expanded(std::vector<std::vector<size_t>>& input_map, std::vector<std::vector<std::vector<size_t>>>& output_time_expanded_draph, size_t index_in_output, std::pair<int, int> agent, size_t time) {
	
	std::vector<std::vector<size_t>> temp_map(input_map.size(), std::vector<size_t>(input_map[0].size(), 0));

	std::queue<std::pair<size_t, size_t>> vertex_queue;

	std::vector<size_t> new_vertices_in_time;

	size_t t = 0;
	vertex_queue.push(agent);
	temp_map[agent.first][agent.second] = 1;
	new_vertices_in_time.push_back(input_map[agent.first][agent.second]);

	while (t < time && !vertex_queue.empty()) {

		//push lap delimiter
		vertex_queue.push(std::make_pair(0, 0));

		std::sort(new_vertices_in_time.begin(), new_vertices_in_time.end());
		output_time_expanded_draph[index_in_output].push_back(new_vertices_in_time);
		new_vertices_in_time.clear();

		while (vertex_queue.front().first != 0) {
			std::pair<size_t, size_t> a = vertex_queue.front();
			vertex_queue.pop();

			if (input_map[a.first - 1][a.second] != 0 && temp_map[a.first - 1][a.second] != 1) {
				temp_map[a.first - 1][a.second] = 1;
				vertex_queue.push(std::make_pair(a.first - 1, a.second));
				new_vertices_in_time.push_back(input_map[a.first - 1][a.second]);
			}
			if (input_map[a.first][a.second + 1] != 0 && temp_map[a.first][a.second + 1] != 1) {
				temp_map[a.first][a.second + 1] = 1;
				vertex_queue.push(std::make_pair(a.first, a.second + 1));
				new_vertices_in_time.push_back(input_map[a.first][a.second + 1]);
			}
			if (input_map[a.first + 1][a.second] != 0 && temp_map[a.first + 1][a.second] != 1) {
				temp_map[a.first + 1][a.second] = 1;
				vertex_queue.push(std::make_pair(a.first + 1, a.second));
				new_vertices_in_time.push_back(input_map[a.first + 1][a.second]);
			}
			if (input_map[a.first][a.second - 1] != 0 && temp_map[a.first][a.second - 1] != 1) {
				temp_map[a.first][a.second - 1] = 1;
				vertex_queue.push(std::make_pair(a.first, a.second - 1));
				new_vertices_in_time.push_back(input_map[a.first][a.second - 1]);
			}
		}

		//pop lap delimiter
		vertex_queue.pop();
		t++;
	}

	while (t < time) {
		output_time_expanded_draph[index_in_output].push_back(std::vector<size_t>());
		t++;
	}
}

/** Computes time expanded graph for group of agents.
*
* Multithreading is used for computing.
*
* @param input_map reference map.
* @param output_time_expanded_draph pair of vectors for writing the result for agents.
* @param agents vector of pairs containig agent's start and finish coordinates in pair.
* @param time makespan for time expanded graph.
* @return error message, "OK" if everything ended right.
*/
std::string time_expanded_multiagent(std::vector<std::vector<size_t>>& input_map, std::pair<std::vector<std::vector<std::vector<size_t>>>, std::vector<std::vector<std::vector<size_t>>>>& output_time_expanded_draph, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>& agents, size_t time) {

	if (output_time_expanded_draph.first.size() != agents.size() || 
		output_time_expanded_draph.second.size() != agents.size()) {
		return "ERROR: different lenghts of time_expanded and agents\n";
	}

	std::vector<std::thread> threads;

	give_new_numbering(input_map);

	/*
	for (size_t i = 0; i < agents.size(); i++) {
		time_expanded(std::ref(input_map), std::ref(output_time_expanded_draph.first), i, agents[i].first, time);
		time_expanded(std::ref(input_map), std::ref(output_time_expanded_draph.second), i, agents[i].second, time);
	}
	*/

	//Launch threads
	for (size_t i = 0; i < agents.size(); i++) {
		threads.push_back(std::thread(time_expanded, std::ref(input_map), std::ref(output_time_expanded_draph.first), i, agents[i].first, time));
		threads.push_back(std::thread(time_expanded, std::ref(input_map), std::ref(output_time_expanded_draph.second), i, agents[i].second, time));
	}

	//Join the threads with the main thread
	for (auto& thread : threads) {
		thread.join();
	}

	return "OK";
}