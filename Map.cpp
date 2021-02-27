#include "map.hpp"

/** Default constructor.*/
Map::Map() {}

/** Constructor with files.
* 
* @param map_file_name file containing map.
* @param agents_file_name file containing agents.
*/
Map::Map(std::string map_file_name, std::string agents_file_name): 
	map_file_name(map_file_name), 
	agents_file_name(agents_file_name) 
{}

/** Setting file name containig map.
*
* @param new_map_file_name file containing map.
*/
void Map::set_map_file(std::string new_map_file_name) {
	map_file_name = new_map_file_name;
}

/** Setting file name containig agents.
*
* @param new_agents_file_name file containing agents.
*/
void Map::set_agents_file(std::string new_agents_file_name) {
	agents_file_name = new_agents_file_name;
}

/** Loading map from file.
*
* @param custom_map_file_name custom file containing map, optional.
* @return error message, "OK" if everything ended right.
*/
std::string Map::load_map(std::string custom_map_file_name) {

	std::ifstream map_file;
	if (custom_map_file_name != "") {
		map_file = std::ifstream(custom_map_file_name);
	}
	else {
		map_file = std::ifstream(map_file_name);
	}

	if (map_file.is_open()) {
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
	}
	else {
		if (custom_map_file_name != "") {
			return "ERROR: cannot open file for reading: " + custom_map_file_name + "\n";
		}
		else {
			return "ERROR: cannot open file for reading: " + map_file_name + "\n";
		}
	}

	map_file.close();

	return "OK";
}

/** Loading agents from file.
*
* @param custom_agents_file_name custom file containing agents, optional.
* @return error message, "OK" if everything ended right.
*/
std::string Map::load_agents(std::string custom_agents_file_name) {

	std::ifstream agents_file;
	if (custom_agents_file_name != "") {
		agents_file = std::ifstream(custom_agents_file_name);
	}
	else {
		agents_file = std::ifstream(agents_file_name);
	}

	if (agents_file.is_open()) {
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
		if (custom_agents_file_name != "") {
			return "ERROR: cannot open file for reading: " + custom_agents_file_name + "\n";
		}
		else {
			return "ERROR: cannot open file for reading: " + agents_file_name + "\n";
		}
	}

	agents_file.close();

	agents_shortest_paths.resize(agents.size());
	time_expanded_graph.resize(agents.size());

	return "OK";
}

/** Loading map and agents from files.
*
* @return error message, "OK" if everything ended right.
*/
std::string Map::reload() {

	auto ret_load_map = load_map();
	auto ret_load_agents = load_agents();

	std::string ret = "";
	if (ret_load_map != "OK") {
		ret += ret_load_map;
	}
	if (ret_load_agents != "OK") {
		ret += ret_load_agents;
	}

	if (ret == "") {
		return "OK";
	}
	else {
		return ret;
	}
}

/** Generates graph and agents output.
*
* @param ofile output file stream for writing.
* @return number of vertices.
*/
size_t Map::make_graph_and_agents_output(std::ofstream& ofile) {

	size_t vertex_number = 0;

	ofile << "    Graph = [" << std::endl;

	for (size_t i = 1; i <= height + 1; i++) {
		for (size_t j = 0; j < width; j++) {
			if (computed_map[i][j + 1] != 0 && i != height + 1) {
				vertex_number++;
				computed_map[i][j + 1] = vertex_number;
			}
			if (i > 1) {
				if (computed_map[i - 1][j + 1] != 0) {
					if (computed_map[i - 1][j + 1] != 1) {
						ofile << "," << std::endl;
					}
					ofile << "    $neibs(" << computed_map[i - 1][j + 1] << ",[" << computed_map[i - 1][j + 1];
					if (computed_map[i - 2][j + 1] != 0) {
						ofile << "," << computed_map[i - 2][j + 1];
					}
					if (computed_map[i - 1][j + 2] != 0) {
						ofile << "," << computed_map[i - 1][j + 2];
					}
					if (computed_map[i][j + 1] != 0) {
						ofile << "," << computed_map[i][j + 1];
					}
					if (computed_map[i - 1][j] != 0) {
						ofile << "," << computed_map[i - 1][j];
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
		ofile << "(" << computed_map[asy][asx] << "," << computed_map[aey][aex] << ")";
	}

	ofile << "]," << std::endl;

	return vertex_number;
}

/** Generates output from preprocessing.
*
* @param ofile output file stream for writing.
* @param time makespan
* @param number_of_vertices number of vertices in map
*/
void Map::make_preprocessed_output(std::ofstream& ofile, size_t time, size_t number_of_vertices) {

	std::vector<size_t> seen_vertices;
	size_t index = 0;

	for (size_t a = 0; a < time_expanded_graph.size(); a++) {

		seen_vertices.clear();
		for (size_t t = 0; t < time; t++) {

			if (t < time_expanded_graph[a].size()) {
				std::vector<size_t> temp = std::move(seen_vertices);
				std::merge(temp.begin(), temp.end(), time_expanded_graph[a][t].begin(), time_expanded_graph[a][t].end(), std::back_inserter(seen_vertices));
			}
				
			index = 0;

			for (size_t v = 1; v <= number_of_vertices; v++) {
				if (v > seen_vertices.back() || v != seen_vertices[index]) {
					ofile << "," << std::endl;
					ofile << "    B[" << t + 1 << "," << a + 1 << "," << v << "] = 0";
				}
				else {
					index++;
				}
			}
		}
	}

}

/** Generates input for picat without preprocessing.
*
* Generates from Map::computed_map.
* 
* @param output_file_name file for writing.
* @return error message, "OK" if everything ended right.
*/
std::string Map::make_output_without_preprocessing(std::string output_file_name) {

	std::ofstream ofile;
	ofile.open(output_file_name);

	if (ofile.is_open()) {

		ofile << "ins(Graph, As, Avoid, Makespan, SumOfCosts) =>" << std::endl;

		make_graph_and_agents_output(ofile);

		ofile << "    Avoid = new_array(0,0)," << std::endl;
		ofile << "    Makespan = -1," << std::endl;
		ofile << "    SumOfCosts = -1.";
	}
	else {
		return "ERROR: cannot open file for writing: " + output_file_name + "\n";
	}

	ofile.close();

	return "OK";
}

/** Generates input for picat with preprocessing.
* 
* Generates from Map::computed_map, Map::time_expanded_graph and Map::agents_shortest_paths if time is default.
*
* @param output_file_name file for writing.
* @param time default = longest of the shortest paths.
* @return error message, "OK" if everything ended right.
*/
std::string Map::make_output(std::string output_file_name, size_t time) {

	std::ofstream ofile;
	ofile.open(output_file_name);

	if (ofile.is_open()) {

		if (time == 0) {
			for (size_t i = 0; i < agents_shortest_paths.size(); i++) {
				if (agents_shortest_paths[i].size() > time) {
					time = agents_shortest_paths[i].size();
				}
			}
		}

		ofile << "ins(Graph, As, B) =>" << std::endl;

		size_t number_of_vertices = make_graph_and_agents_output(ofile);

		ofile << "    B = new_array(" << time << "," << agents.size() << "," << number_of_vertices << ")";

		make_preprocessed_output(ofile, time, number_of_vertices);

		ofile << ".";
	}
	else {
		return "ERROR: cannot open file for writing: " + output_file_name + "\n";
	}

	ofile.close();

	return "OK";
}

/** Zeroes computed map and mirrors sizes of loaded map.*/
void Map::reset_computed_map() {

	computed_map = std::vector<std::vector<size_t>>(map.size(), std::vector<size_t>(map[0].size(), 0));
}
