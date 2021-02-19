#include "Algorithms.hpp"

void BFS(std::vector<std::vector<size_t>>& map_input, std::vector<std::vector<size_t>>& map_output, std::pair<std::pair<int, int>, std::pair<int, int>> agent) {

	std::vector<std::vector<size_t>> temp_map(map_input.size(), std::vector<size_t>(map_input[0].size(), 0));

	std::queue<std::pair<size_t, size_t>> vertex_queue, next_lap;

	bool found = false;
	next_lap.push(agent.first);

	while (!(found || next_lap.empty())) {

		vertex_queue = next_lap;
		next_lap = std::queue<std::pair<size_t, size_t>>();

		while (!vertex_queue.empty()) {
			std::pair<size_t, size_t> a = vertex_queue.front();
			vertex_queue.pop();

			if (a.first == agent.second.first && a.second == agent.second.second) {
				found = true;
			}

			//map_output[a.first][a.second] = 1;
			temp_map[a.first][a.second] = 2;
			//map_dump(temp_map, "debug.txt");

			if (map_input[a.first - 1][a.second] != 0 && temp_map[a.first - 1][a.second] != 2) {
				//temp_map[a.first - 1][a.second] = 2;
				next_lap.push(std::make_pair(a.first - 1, a.second));
			}
			if (map_input[a.first][a.second + 1] != 0 && temp_map[a.first][a.second + 1] != 2) {
				//temp_map[a.first][a.second + 1] = 2;
				next_lap.push(std::make_pair(a.first, a.second + 1));
			}
			if (map_input[a.first + 1][a.second] != 0 && temp_map[a.first + 1][a.second] != 2) {
				//temp_map[a.first + 1][a.second] = 2;
				next_lap.push(std::make_pair(a.first + 1, a.second));
			}
			if (map_input[a.first][a.second - 1] != 0 && temp_map[a.first][a.second - 1] != 2) {
				//temp_map[a.first][a.second - 1] = 2;
				next_lap.push(std::make_pair(a.first, a.second - 1));
			}
		}
	}

	//map_dump(temp_map, "debug.txt");


	found = false;
	next_lap = std::queue<std::pair<size_t, size_t>>();
	next_lap.push(agent.second);

	while (!(found || next_lap.empty())) {

		vertex_queue = next_lap;
		next_lap = std::queue<std::pair<size_t, size_t>>();

		while (!vertex_queue.empty()) {
			std::pair<size_t, size_t> a = vertex_queue.front();
			vertex_queue.pop();

			if (a.first == agent.first.first && a.second == agent.first.second) {
				found = true;
			}
		
			if (temp_map[a.first][a.second] % 2 == 0) {
				temp_map[a.first][a.second] += 1;
			}
			
			if (temp_map[a.first][a.second] == 3) {
				map_output[a.first][a.second] = 1;
			}

			if (map_input[a.first - 1][a.second] != 0 && temp_map[a.first - 1][a.second] % 2 == 0) {
				next_lap.push(std::make_pair(a.first - 1, a.second));
			}
			if (map_input[a.first][a.second + 1] != 0 && temp_map[a.first][a.second + 1] % 2 == 0) {
				next_lap.push(std::make_pair(a.first, a.second + 1));
			}
			if (map_input[a.first + 1][a.second] != 0 && temp_map[a.first + 1][a.second] % 2 == 0) {
				next_lap.push(std::make_pair(a.first + 1, a.second));
			}
			if (map_input[a.first][a.second - 1] != 0 && temp_map[a.first][a.second - 1] % 2 == 0) {
				next_lap.push(std::make_pair(a.first, a.second - 1));
			}
		}
	}

	//map_dump(temp_map, "debug.txt");
}

void BFS_multithread(std::vector<std::vector<size_t>>& map_input, std::vector<std::vector<size_t>>& map_output, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> agents) {
	std::vector<std::thread> threads;

	//Launch threads
	for (size_t i = 0; i < agents.size(); i++) {
		threads.push_back(std::thread(BFS, std::ref(map_input), std::ref(map_output), agents[i]));
	}

	//Join the threads with the main thread
	for (auto& thread : threads) {
		thread.join();
	}

}