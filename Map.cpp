#include "map.hpp"

Map::Map(std::string map_file_name, std::string agents_file_name): 
	map_file_name(map_file_name), 
	agents_file_name(agents_file_name) 
{}

void Map::set_map_file(std::string new_map_file_name) {
	map_file_name = new_map_file_name;
}

void Map::set_agents_file(std::string new_agents_file_name) {
	agents_file_name = new_agents_file_name;
}

int Map::load_map() {
	std::ifstream map_file(map_file_name);
	std::ifstream agents_file(agents_file_name);
	std::ofstream ofile;
	ofile.open("output.txt");

	if (map_file.is_open() && agents_file.is_open()) {
		std::string map_line;
		size_t height, width;
		size_t vertex_number = 1;

		std::getline(map_file, map_line); //first line is unnecessary

		std::getline(map_file, map_line); //second line
		height = atoi(&map_line[height_number_index]);

		std::getline(map_file, map_line); //third line
		width = atoi(&map_line[width_number_index]);

		std::getline(map_file, map_line); //fourth line is unnecessary

		std::vector<std::vector<size_t>> graph(height + 2);
		graph[0].resize(width + 2, 0);//top bound
		graph[height + 1].resize(width + 2, 0);//bot bound

		ofile << "ins(Graph, As, Avoid, Makespan, SumOfCosts) =>" << std::endl;
		ofile << "    Graph = [" << std::endl;

		for (size_t i = 1; i <= height + 1; i++) { //reading input
			if (i != height + 1) {
				std::getline(map_file, map_line);
				graph[i].resize(width + 2, 0);
			}
			for (size_t j = 0; j < width; j++) {
				if (map_line[j] == '.' && i != height + 1) {
					graph[i][j + 1] = vertex_number;
					vertex_number++;
				}
				if (i > 1) {
					if (graph[i - 1][j + 1] != 0) {
						if (graph[i - 1][j + 1] != 1) {
							ofile << "," << std::endl;
						}
						ofile << "    $neibs(" << graph[i - 1][j + 1] << ",[" << graph[i - 1][j + 1];
						if (graph[i - 2][j + 1] != 0) {
							ofile << "," << graph[i - 2][j + 1];
						}
						if (graph[i - 1][j + 2] != 0) {
							ofile << "," << graph[i - 1][j + 2];
						}
						if (graph[i][j + 1] != 0) {
							ofile << "," << graph[i][j + 1];
						}
						if (graph[i - 1][j] != 0) {
							ofile << "," << graph[i - 1][j];
						}
						ofile << "])";
					}
				}
			}
		}

		ofile << std::endl;
		ofile << "    ]," << std::endl;
		ofile << "    As = [";

		std::string agents_line;
		std::getline(agents_file, agents_line); //first line unimportent

		size_t agent_index = 0;
		while (std::getline(agents_file, agents_line)) {

			std::stringstream tokenizer(agents_line);
			std::string column;
			size_t asx, asy, aex, aey;
			tokenizer >> column;
			for (size_t i = 0; i < 9; i++, tokenizer >> column) {
				switch (i) {
				case 4:
					asx = std::stoi(column);
					break;
				case 5:
					asy = std::stoi(column);
					break;
				case 6:
					aex = std::stoi(column);
					break;
				case 7:
					aey = std::stoi(column);
					break;
				default:
					break;
				}
			}

			if (agent_index != 0) {
				ofile << ",";
			}
			ofile << "(" << graph[asy][asx] << "," << graph[aey][aex] << ")";
			agent_index++;
		}

		ofile << "]," << std::endl;
		ofile << "    Avoid = new_array(0,0)," << std::endl;
		ofile << "    Makespan = -1," << std::endl;
		ofile << "    SumOfCosts = -1." << std::endl;
	}

	map_file.close();
	agents_file.close();
	ofile.close();

	return 0;
}