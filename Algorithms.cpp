#include "Algorithms.hpp"

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
		//map_dump(map_output, "debug.txt");
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

		//map_dump(map_output, "debug.txt");
	}
}

void time_expanded_multithread(std::vector<std::vector<size_t>>& reference_map, std::vector<std::vector<size_t>>& map_output, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>& agents) {
	
	std::vector<std::thread> threads;

	//Launch threads
	for (size_t i = 0; i < agents.size(); i++) {
		threads.push_back(std::thread(time_expanded, std::ref(reference_map), std::ref(map_output), agents[i]));
	}

	//Join the threads with the main thread
	for (auto& thread : threads) {
		thread.join();
	}
}

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

void shortest_path_multithread(std::vector<std::vector<size_t>>& reference_map, std::vector<std::vector<std::pair<size_t, size_t>>>& output_paths, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>& agents) {

	if (output_paths.size() != agents.size()) {
		std::cout << "ERROR: different lenghts of paths and agents\n";
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
}

void paths_to_map(std::vector<std::vector<std::pair<size_t, size_t>>>& input_paths, std::vector<std::vector<size_t>>& map_output) {

	for (size_t i = 0; i < input_paths.size(); i++) {
		for (size_t j = 0; j < input_paths[i].size(); j++) {
			map_output[input_paths[i][j].first][input_paths[i][j].second] = 1;
		}
	}
}

void map_union(std::vector<std::vector<size_t>>& map1, std::vector<std::vector<size_t>>& map2, std::vector<std::vector<size_t>>& map_output) {

	if (map1.size() != map2.size() || map1[0].size() != map2[0].size()) {
		std::cout << "ERROR: different lenghts of maps in union\n";
	}

	for (size_t i = 0; i < map1.size(); i++) {
		for (size_t j = 0; j < map1[0].size(); j++) {
			map_output[i][j] = map1[i][j] | map2[i][j];
		}
	}
}

void map_intersection(std::vector<std::vector<size_t>>& map1, std::vector<std::vector<size_t>>& map2, std::vector<std::vector<size_t>>& map_output) {

	if (map1.size() != map2.size() || map1[0].size() != map2[0].size()) {
		std::cout << "ERROR: different lenghts of maps in union\n";
	}

	for (size_t i = 0; i < map1.size(); i++) {
		for (size_t j = 0; j < map1[0].size(); j++) {
			map_output[i][j] = map1[i][j] & map2[i][j];
		}
	}
}

void add_free_surroundings(std::vector<std::vector<size_t>>& reference_map, std::vector<std::vector<size_t>>& map_to_surround, std::vector<std::vector<size_t>>& map_output) {

	bool same = false;
	std::vector<std::vector<size_t>> copy;
	if (&map_to_surround == &map_output) {
		same = true;
		copy = map_to_surround;
	}

	bool surround = false;
	for (size_t i = 0; i < reference_map.size(); i++) {
		for (size_t j = 0; j < reference_map[0].size(); j++) {
			
			if (same) {
				if (copy[i][j] != 0) {
					surround = true;
				}
			}
			else if (map_to_surround[i][j] != 0) {
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
}