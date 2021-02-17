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

int Map::load_map_and_agents() {
	std::ifstream map_file(map_file_name);
	std::ifstream agents_file(agents_file_name);

	if (map_file.is_open() && agents_file.is_open()) {
		std::string map_line;

		std::getline(map_file, map_line); //first line is unnecessary

		std::getline(map_file, map_line); //second line
		height = atoi(&map_line[height_number_index]);

		std::getline(map_file, map_line); //third line
		width = atoi(&map_line[width_number_index]);

		std::getline(map_file, map_line); //fourth line is unnecessary

		map = std::vector<std::vector<size_t>>(height + 2);
		map[0].resize(width + 2, 0); //top bound
		map[height + 1].resize(width + 2, 0); //bot bound

		size_t vertex_number = 1;

		for (size_t i = 1; i <= height + 1; i++) { //reading input
			if (i != height + 1) {
				std::getline(map_file, map_line);
				map[i].resize(width + 2, 0);
			}
			for (size_t j = 0; j < width; j++) {
				if (map_line[j] == '.' && i != height + 1) {
					map[i][j + 1] = vertex_number;
					vertex_number++;
				}
			}
		}

		
		std::string agents_line;
		std::getline(agents_file, agents_line); //first line unimportent

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

			agents.push_back(std::make_pair(std::make_pair(asy, asx), std::make_pair(aey, aex)));
		}
	}
	else {
		return -1;
	}

	map_file.close();
	agents_file.close();

	return 0;
}

int Map::make_output(std::string output_file_name) {
	std::ofstream ofile;
	ofile.open(output_file_name);

	size_t vertex_number = 1;

	ofile << "ins(Graph, As, Avoid, Makespan, SumOfCosts) =>" << std::endl;
	ofile << "    Graph = [" << std::endl;

	for (size_t i = 1; i <= height + 1; i++) {
		for (size_t j = 0; j < width; j++) {
			if (map[i][j + 1] != 0 && i != height + 1) {
				map[i][j + 1] = vertex_number;
				vertex_number++;
			}
			if (i > 1) {
				if (map[i - 1][j + 1] != 0) {
					if (map[i - 1][j + 1] != 1) {
						ofile << "," << std::endl;
					}
					ofile << "    $neibs(" << map[i - 1][j + 1] << ",[" << map[i - 1][j + 1];
					if (map[i - 2][j + 1] != 0) {
						ofile << "," << map[i - 2][j + 1];
					}
					if (map[i - 1][j + 2] != 0) {
						ofile << "," << map[i - 1][j + 2];
					}
					if (map[i][j + 1] != 0) {
						ofile << "," << map[i][j + 1];
					}
					if (map[i - 1][j] != 0) {
						ofile << "," << map[i - 1][j];
					}
					ofile << "])";
				}
			}
		}
	}

	ofile << std::endl;
	ofile << "    ]," << std::endl;
	ofile << "    As = [";

	for (size_t i = 0; i < agents.size(); i++) {
		if (i != 0) {
			ofile << ",";
		}
		int asy = agents[i].first.first;
		int asx = agents[i].first.second;
		int aey = agents[i].second.first;
		int aex = agents[i].second.second;
		ofile << "(" << map[asy][asx] << "," << map[aey][aex] << ")";
	}

	ofile << "]," << std::endl;
	ofile << "    Avoid = new_array(0,0)," << std::endl;
	ofile << "    Makespan = -1," << std::endl;
	ofile << "    SumOfCosts = -1." << std::endl;

	ofile.close();

	return 0;
}