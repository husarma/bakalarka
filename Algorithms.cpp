#include "Algorithms.hpp"

void BFS(std::vector<std::vector<size_t>>& map_input, std::vector<std::vector<size_t>>& map_output, std::pair<std::pair<int, int>, std::pair<int, int>> agent) {

	std::vector<std::vector<size_t>> temp_map(map_input.size(), std::vector<size_t>(map_input[0].size(), 0));

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

			//temp_map[a.first][a.second] = 2;

			if (map_input[a.first - 1][a.second] != 0 && temp_map[a.first - 1][a.second] != 2) {
				temp_map[a.first - 1][a.second] = 2;
				vertex_queue.push(std::make_pair(a.first - 1, a.second));
			}
			if (map_input[a.first][a.second + 1] != 0 && temp_map[a.first][a.second + 1] != 2) {
				temp_map[a.first][a.second + 1] = 2;
				vertex_queue.push(std::make_pair(a.first, a.second + 1));
			}
			if (map_input[a.first + 1][a.second] != 0 && temp_map[a.first + 1][a.second] != 2) {
				temp_map[a.first + 1][a.second] = 2;
				vertex_queue.push(std::make_pair(a.first + 1, a.second));
			}
			if (map_input[a.first][a.second - 1] != 0 && temp_map[a.first][a.second - 1] != 2) {
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
		
			//if (temp_map[a.first][a.second] % 2 == 0) {
			//	temp_map[a.first][a.second] += 1;
			//}
			
			if (temp_map[a.first][a.second] == 3) {
				map_output[a.first][a.second] = 1;
			}

			if (map_input[a.first - 1][a.second] != 0 && temp_map[a.first - 1][a.second] % 2 == 0) {
				if (temp_map[a.first - 1][a.second] % 2 == 0) {
					temp_map[a.first - 1][a.second] += 1;
				}
				vertex_queue.push(std::make_pair(a.first - 1, a.second));
			}
			if (map_input[a.first][a.second + 1] != 0 && temp_map[a.first][a.second + 1] % 2 == 0) {
				if (temp_map[a.first][a.second + 1] % 2 == 0) {
					temp_map[a.first][a.second + 1] += 1;
				}
				vertex_queue.push(std::make_pair(a.first, a.second + 1));
			}
			if (map_input[a.first + 1][a.second] != 0 && temp_map[a.first + 1][a.second] % 2 == 0) {
				if (temp_map[a.first + 1][a.second] % 2 == 0) {
					temp_map[a.first + 1][a.second] += 1;
				}
				vertex_queue.push(std::make_pair(a.first + 1, a.second));
			}
			if (map_input[a.first][a.second - 1] != 0 && temp_map[a.first][a.second - 1] % 2 == 0) {
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