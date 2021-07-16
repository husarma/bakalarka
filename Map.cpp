#include "map.hpp"

#include <mutex>
#include <filesystem>

/** Default constructor.*/
Map::Map() {}

/** Constructor with files.
*
* @param map_file_name file containing map.
* @param agents_file_name file containing agents.
*/
Map::Map(std::string map_file_name, std::string agents_file_name) :
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

/** Setting output strategy.
*
* @param out output strategy.
*/
void Map::set_outputter(std::unique_ptr<OutputterStrategyI> out) {
	outputter = std::move(out);
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

	size_t vertex_number = 0;

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

		vertex_number++;

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

	original_number_of_vertices = vertex_number;

	map_file.close();

	return "OK";
}

/** Loading agents from file.
*
* @param number_of_agents max number of agents to be loaded, optional.
* @param custom_agents_file_name custom file containing agents, optional.
* @return error message, "OK" if everything ended right.
*/
std::string Map::load_agents(int number_of_agents, std::string custom_agents_file_name) {

	agents.clear();

	std::ifstream agents_file;
	if (custom_agents_file_name != "") {
		agents_file = std::ifstream(custom_agents_file_name);
	}
	else {
		agents_file = std::ifstream(agents_file_name);
	}

	if (number_of_agents == -1) {
		number_of_agents = INT_MAX;
	}

	if (agents_file.is_open()) {
		std::string agents_line;
		std::getline(agents_file, agents_line); //first line unimportant

		size_t agents_loaded = 0;

		while (agents_loaded < number_of_agents && std::getline(agents_file, agents_line)) {

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

			agents.push_back(std::make_pair(std::make_pair(asy + 1, asx + 1), std::make_pair(aey + 1, aex + 1)));
			agents_loaded++;
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

	agents_shortest_paths.clear();
	time_expanded_graph.first.clear();
	time_expanded_graph.second.clear();

	agents_shortest_paths.resize(agents.size());
	time_expanded_graph.first.resize(agents.size());
	time_expanded_graph.second.resize(agents.size());

	return "OK";
}

/** Loading map and agents from files.
*
* @param number_of_agents max number of agents to be loaded, optional.
* @return error message, "OK" if everything ended right.
*/
std::string Map::reload(int number_of_agents) {

	auto ret_load_map = load_map();
	auto ret_load_agents = load_agents(number_of_agents);

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

/** Call Picat.
*
* Prepare input and output files for picat and call him for solving, then analyze output.
*
* @param log_file file for logging.
* @param alg name of an algorithm.
* @return pair of error message, "OK" if that instance have solution, and informations about computing in tuple: base_map_name, base_agents_name, number_of_vertices, number_of_agents.
*/
std::pair<std::string, std::tuple<std::string, std::string, size_t, size_t>> Map::picat(std::string log_file, std::string alg) {

	std::filesystem::create_directory("temp");

	std::string relative_dir_wind = "temp\\";
	std::string relative_dir_unix = "temp/";
	
	
	std::string base_map_name = map_file_name.substr(map_file_name.find_last_of("/\\") + 1);
	std::string::size_type const map_dot(base_map_name.find_last_of('.'));

	std::string base_agents_name = agents_file_name.substr(agents_file_name.find_last_of("/\\") + 1);
	std::string::size_type const agents_dot(base_agents_name.find_last_of('.'));

	std::string file_name = base_map_name.substr(0, map_dot) + "_" + base_agents_name.substr(0, agents_dot) + "_" + std::to_string(agents.size()) + "_" + std::to_string(time_expanded_graph.first[0].size());
	
	std::string picat_input = file_name + alg + "_in.pi";
	std::string picat_output = file_name + alg + "_out.pi";

	outputter->make_output(relative_dir_wind + picat_input, this);

	std::fstream fs;
	fs.open(relative_dir_wind + picat_output, std::ios::out);
	fs.close();

	std::stringstream exec;
	exec << "bash -c \"timeout 5m ./picat mks_allowed_list.pi " << relative_dir_unix + picat_input << " > " << relative_dir_unix + picat_output << "\""; //5-10 min statndard
	auto x = exec.str();

	std::string out = base_map_name + "\t" + base_agents_name + "\t" + std::to_string(number_of_vertices) + "\t" + std::to_string(agents.size()) + "\t" + std::to_string(time_expanded_graph.first[0].size()) + "\t";
	std::ofstream ofile;
	ofile.open(log_file, std::ios::app);
	if (ofile.is_open()) {
		ofile << out;
	}

	system(x.c_str());

	std::mutex m;

	//reading output
	std::ifstream picat_result;
	picat_result.open(relative_dir_wind + picat_output);

	if (picat_result.is_open()) {

		std::string line;
		size_t line_number = 0;
		while (std::getline(picat_result, line)) {
			line_number++;
			if (line_number == 4) {
				if (line == "agents | timesteps") {
					if (ofile.is_open()) {
						ofile << "OK" << std::endl;
					}
					m.lock();
					std::cout << alg + "\t" << out << "OK" << std::endl;
					m.unlock();
					ofile.close();
					return std::make_pair("OK", std::make_tuple(base_map_name, base_agents_name, number_of_vertices, agents.size()));
				}
				else {
					break;
				}
			}
		}

		if (line_number > 0) {
			if (ofile.is_open()) {
				ofile << "NO solution" << std::endl;
			}
			m.lock();
			std::cout << alg + "\t" << out << "NO solution" << std::endl;
			m.unlock();
			ofile.close();
			return std::make_pair("NO solution", std::make_tuple(base_map_name, base_agents_name, number_of_vertices, agents.size()));
		}
		if (ofile.is_open()) {
			ofile << "Timed out" << std::endl;
		}
		m.lock();
		std::cout << alg + "\t" << out << "Timed out" << std::endl;
		m.unlock();
		ofile.close();
		return std::make_pair("Timed out", std::make_tuple(base_map_name, base_agents_name, number_of_vertices, agents.size()));
	}
	else {
		ofile.close();
		return std::make_pair("ERROR: cannot open file for reading result : " + relative_dir_wind + picat_output + "\n", std::make_tuple(base_map_name, base_agents_name, number_of_vertices, agents.size()));
	}
}

/** Computes minimal required time for agents to complete its paths.
*
* Computes from Map::agents_shortest_paths.
*
* @return lenght of the logest from the shortest paths.
*/
size_t Map::get_min_time() {
	size_t max = 0;
	for (size_t a = 0; a < agents_shortest_paths.size(); a++) {
		if (agents_shortest_paths[a].size() > max) {
			max = agents_shortest_paths[a].size();
		}
	}
	return max;
}

/** Zeroes computed map and mirrors sizes of loaded map.*/
void Map::reset_computed_map() {

	computed_map = std::vector<std::vector<size_t>>(map.size(), std::vector<size_t>(map[0].size(), 0));
}

/** Generates graph and agents input for Picat.
*
* Template method design pattern.
*
* @param ofile output file stream for writing.
* @param map to generate from.
* @return number of vertices.
*/
size_t OutputterStrategyI::make_graph_and_agents_output(std::ofstream& ofile, Map* map) {

	size_t vertex_number = 0;

	ofile << "Graph = [" << " ";

	for (size_t i = 1; i <= map->height + 1; i++) {
		for (size_t j = 0; j < map->width; j++) {
			if (map->computed_map[i][j + 1] != 0 && i != map->height + 1) {
				vertex_number++;
				map->computed_map[i][j + 1] = vertex_number;
			}
			if (i > 1) {
				if (map->computed_map[i - 1][j + 1] != 0) {
					if (map->computed_map[i - 1][j + 1] != 1) {
						ofile << "," << " ";
					}
					ofile << "$neibs(" << map->computed_map[i - 1][j + 1] << ",[" << map->computed_map[i - 1][j + 1];
					if (map->computed_map[i - 2][j + 1] != 0) {
						ofile << "," << map->computed_map[i - 2][j + 1];
					}
					if (map->computed_map[i - 1][j + 2] != 0) {
						ofile << "," << map->computed_map[i - 1][j + 2];
					}
					if (map->computed_map[i][j + 1] != 0) {
						ofile << "," << map->computed_map[i][j + 1];
					}
					if (map->computed_map[i - 1][j] != 0) {
						ofile << "," << map->computed_map[i - 1][j];
					}
					ofile << "])";
				}
			}
		}
	}

	ofile << " ";
	ofile << "]," << " ";
	ofile << "As = [";

	for (size_t i = 0; i < map->agents.size(); i++) {
		if (i != 0) {
			ofile << ",";
		}
		size_t asy = map->agents[i].first.first;
		size_t asx = map->agents[i].first.second;
		size_t aey = map->agents[i].second.first;
		size_t aex = map->agents[i].second.second;
		ofile << "(" << map->computed_map[asy][asx] << "," << map->computed_map[aey][aex] << ")";
	}

	ofile << "]," << " ";

	map->number_of_vertices = vertex_number;
	return vertex_number;
}

/** Generates input for Picat.
*
* @param output_file_name output file for writing.
* @param map to generate from.
* @return error message, "OK" if everything ended right.
*/
std::string OWithoutPredpocess::make_output(std::string output_file_name, Map* map) {
	std::ofstream ofile;
	ofile.open(output_file_name);

	if (ofile.is_open()) {

		ofile << "ins(Graph, As, Avoid, Makespan, SumOfCosts) =>" << " ";

		make_graph_and_agents_output(ofile, map);

		ofile << "Avoid = new_array(0,0)," << " ";
		ofile << "Makespan = -1," << " ";
		ofile << "SumOfCosts = -1.";
	}
	else {
		return "ERROR: cannot open file for writing: " + output_file_name + "\n";
	}

	ofile.close();

	return "OK";
}

/** Generates input for Picat.
*
* @param output_file_name output file for writing.
* @param map to generate from.
* @return error message, "OK" if everything ended right.
*/
std::string OPreprocess1::make_output(std::string output_file_name, Map* map) {

	std::ofstream ofile;
	size_t time;
	ofile.open(output_file_name);

	if (ofile.is_open()) {

		time = map->time_expanded_graph.first[0].size();

		ofile << "ins(Graph, As, B) =>" << " ";

		size_t number_of_vertices = make_graph_and_agents_output(ofile, map);

		ofile << "B = new_array(" << time << "," << map->agents.size() /*<< "," << number_of_vertices*/ << ")";

		std::vector<size_t> intersection;
		std::vector<size_t> temp;

		size_t index = 0;

		for (size_t a = 0; a < map->time_expanded_graph.first.size(); a++) {

			intersection.clear();

			for (size_t t = 0; t < time; t++) {

				//merge with new vertices
				temp = std::move(intersection);
				std::merge(temp.begin(), temp.end(), map->time_expanded_graph.first[a][t].begin(), map->time_expanded_graph.first[a][t].end(), std::back_inserter(intersection));

				//cut vertices whitch is too far from finish considering the left time
				if (t > 0) { //in time 0 cutabble vertices doesn't exist
					temp = std::move(intersection);
					std::set_difference(temp.begin(), temp.end(), map->time_expanded_graph.second[a][time - t].begin(), map->time_expanded_graph.second[a][time - t].end(), std::back_inserter(intersection));
				}

				index = 0;

				//1
				for (size_t v = 0; v < intersection.size(); v++) {
					ofile << "," << " ";
					ofile << "B[" << t + 1 << "," << a + 1 << "," << intersection[v] << "] = 1";
				}
			}
		}

		ofile << ".";
	}
	else {
		return "ERROR: cannot open file for writing: " + output_file_name + "\n";
	}

	ofile.close();

	return "OK";
}

/** Generates input for Picat.
*
* @param output_file_name output file for writing.
* @param map to generate from.
* @return error message, "OK" if everything ended right.
*/
std::string OPreprocess0::make_output(std::string output_file_name, Map* map) {

	std::ofstream ofile;
	size_t time;
	ofile.open(output_file_name);

	if (ofile.is_open()) {

		time = map->time_expanded_graph.first[0].size();

		ofile << "ins(Graph, As, B) =>" << " ";

		size_t number_of_vertices = make_graph_and_agents_output(ofile, map);

		ofile << "B = new_array(" << time << "," << map->agents.size() /*<< "," << number_of_vertices*/ << ")";

		std::vector<size_t> intersection;
		std::vector<size_t> temp;

		size_t index = 0;

		for (size_t a = 0; a < map->time_expanded_graph.first.size(); a++) {

			intersection.clear();

			for (size_t t = 0; t < time; t++) {

				//merge with new vertices
				temp = std::move(intersection);
				std::merge(temp.begin(), temp.end(), map->time_expanded_graph.first[a][t].begin(), map->time_expanded_graph.first[a][t].end(), std::back_inserter(intersection));

				//cut vertices whitch is too far from finish considering the left time
				if (t > 0) { //in time 0 cutabble vertices doesn't exist
					temp = std::move(intersection);
					std::set_difference(temp.begin(), temp.end(), map->time_expanded_graph.second[a][time - t].begin(), map->time_expanded_graph.second[a][time - t].end(), std::back_inserter(intersection));
				}

				index = 0;

				//0
				for (size_t v = 1; v <= number_of_vertices; v++) {
					if (v > intersection.back() || v != intersection[index]) {
						ofile << "," << " ";
						ofile << "B[" << t + 1 << "," << a + 1 << "," << v << "] = 0";
					}
					else {
						index++;
					}
				}
			}
		}

		ofile << ".";
	}
	else {
		return "ERROR: cannot open file for writing: " + output_file_name + "\n";
	}

	ofile.close();

	return "OK";
}

/** Generates input for Picat.
*
* @param output_file_name output file for writing.
* @param map to generate from.
* @return error message, "OK" if everything ended right.
*/
std::string OPreprocessForeach0::make_output(std::string output_file_name, Map* map) {

	std::ofstream ofile;
	size_t time;
	ofile.open(output_file_name);

	if (ofile.is_open()) {

		time = map->time_expanded_graph.first[0].size();

		ofile << "ins(Graph, As, B) =>" << " ";

		size_t number_of_vertices = make_graph_and_agents_output(ofile, map);

		ofile << "B = new_array(" << time << "," << map->agents.size() /*<< "," << number_of_vertices*/ << ")";

		std::vector<size_t> intersection;
		std::vector<size_t> temp;

		size_t index = 0;

		for (size_t a = 0; a < map->time_expanded_graph.first.size(); a++) {

			intersection.clear();

			for (size_t t = 0; t < time; t++) {

				//merge with new vertices
				temp = std::move(intersection);
				std::merge(temp.begin(), temp.end(), map->time_expanded_graph.first[a][t].begin(), map->time_expanded_graph.first[a][t].end(), std::back_inserter(intersection));

				//cut vertices whitch is too far from finish considering the left time
				if (t > 0) { //in time 0 cutabble vertices doesn't exist
					temp = std::move(intersection);
					std::set_difference(temp.begin(), temp.end(), map->time_expanded_graph.second[a][time - t].begin(), map->time_expanded_graph.second[a][time - t].end(), std::back_inserter(intersection));
				}

				index = 0;

				//foreach 0
				for (size_t v = 1; v <= number_of_vertices; v++) {
					if (v > intersection.back() || v != intersection[index]) {

						if (v > intersection.back() && v + 3 <= number_of_vertices) {
							ofile << "," << " ";
							ofile << "foreach(V in " << v << ".." << number_of_vertices << ")" << " ";
							ofile << "B[" << t + 1 << "," << a + 1 << ",V] = 0" << " ";
							ofile << "end";

							v = number_of_vertices;
						}
						else if (index <= intersection.size() - 1 && v + 3 < intersection[index]) {
							ofile << "," << " ";
							ofile << "foreach(V in " << v << ".." << intersection[index] - 1 << ")" << " ";
							ofile << "B[" << t + 1 << "," << a + 1 << ",V] = 0" << " ";
							ofile << "end";

							v = intersection[index] - 1;
						}
						else {
							ofile << "," << " ";
							ofile << "B[" << t + 1 << "," << a + 1 << "," << v << "] = 0";
						}
					}
					else {
						index++;
					}
				}
			}
		}

		ofile << ".";
	}
	else {
		return "ERROR: cannot open file for writing: " + output_file_name + "\n";
	}

	ofile.close();

	return "OK";
}

/** Generates input for Picat.
*
* @param output_file_name output file for writing.
* @param map to generate from.
* @return error message, "OK" if everything ended right.
*/
std::string OPreprocessForeach1::make_output(std::string output_file_name, Map* map) {

	std::ofstream ofile;
	size_t time;
	ofile.open(output_file_name);

	if (ofile.is_open()) {

		time = map->time_expanded_graph.first[0].size();

		ofile << "ins(Graph, As, B) =>" << " ";

		size_t number_of_vertices = make_graph_and_agents_output(ofile, map);

		ofile << "B = new_array(" << time << "," << map->agents.size() /*<< "," << number_of_vertices*/ << ")";

		std::vector<size_t> intersection;
		std::vector<size_t> temp;

		size_t index = 0;

		for (size_t a = 0; a < map->time_expanded_graph.first.size(); a++) {

			intersection.clear();

			for (size_t t = 0; t < time; t++) {

				//merge with new vertices
				temp = std::move(intersection);
				std::merge(temp.begin(), temp.end(), map->time_expanded_graph.first[a][t].begin(), map->time_expanded_graph.first[a][t].end(), std::back_inserter(intersection));

				//cut vertices whitch is too far from finish considering the left time
				if (t > 0) { //in time 0 cutabble vertices doesn't exist
					temp = std::move(intersection);
					std::set_difference(temp.begin(), temp.end(), map->time_expanded_graph.second[a][time - t].begin(), map->time_expanded_graph.second[a][time - t].end(), std::back_inserter(intersection));
				}

				index = 0;

				//foreach 1
				std::vector<size_t> out;
				for (size_t v = 0; v <= intersection.size(); v++) {
					if (out.size() == 0 && v < intersection.size()) {
						out.push_back(intersection[v]);
						continue;
					}

					if (v < intersection.size() && intersection[v] == out.back() + 1) {
						out.push_back(intersection[v]);
					}
					else {
						if (out.size() > 3) {
							ofile << "," << " ";
							ofile << "foreach(V in " << v << ".." << out.back() << ")" << " ";
							ofile << "B[" << t + 1 << "," << a + 1 << ",V] = 1" << " ";
							ofile << "end";
						}
						else {
							for (size_t i = 0; i < out.size(); i++) {
								ofile << "," << " ";
								ofile << "B[" << t + 1 << "," << a + 1 << "," << out[i] << "] = 1";
							}
						}
						out.clear();
						out.push_back(intersection[v]);
					}
				}
			}
		}

		ofile << ".";
	}
	else {
		return "ERROR: cannot open file for writing: " + output_file_name + "\n";
	}

	ofile.close();

	return "OK";
}

/** Generates input for Picat.
*
* @param output_file_name output file for writing.
* @param map to generate from.
* @return error message, "OK" if everything ended right.
*/
std::string OPreprocessFlatten1::make_output(std::string output_file_name, Map* map) {

	std::ofstream ofile;
	size_t time;
	ofile.open(output_file_name);

	if (ofile.is_open()) {

		time = map->time_expanded_graph.first[0].size();

		ofile << "ins(Graph, As, B) =>" << " ";

		size_t number_of_vertices = make_graph_and_agents_output(ofile, map);

		ofile << "B = new_array(" << time << "," << map->agents.size() /*<< "," << number_of_vertices*/ << ")";

		std::vector<size_t> intersection;
		std::vector<size_t> temp;

		size_t index = 0;

		for (size_t a = 0; a < map->time_expanded_graph.first.size(); a++) {

			intersection.clear();

			for (size_t t = 0; t < time; t++) {

				//merge with new vertices
				temp = std::move(intersection);
				std::merge(temp.begin(), temp.end(), map->time_expanded_graph.first[a][t].begin(), map->time_expanded_graph.first[a][t].end(), std::back_inserter(intersection));

				//cut vertices whitch is too far from finish considering the left time
				if (t > 0) { //in time 0 cutabble vertices doesn't exist
					temp = std::move(intersection);
					std::set_difference(temp.begin(), temp.end(), map->time_expanded_graph.second[a][time - t].begin(), map->time_expanded_graph.second[a][time - t].end(), std::back_inserter(intersection));
				}

				index = 0;

				//flatten 1
				ofile << ", B[" << t + 1 << "," << a + 1 << "] = flatten([";
				std::vector<size_t> out;
				bool coma = false;
				for (size_t v = 0; v <= intersection.size(); v++) {
					if (out.size() == 0 && v < intersection.size()) {
						out.push_back(intersection[v]);
						continue;
					}

					if (v < intersection.size() && intersection[v] == out.back() + 1) {
						out.push_back(intersection[v]);
					}
					else {
						if (out.size() > 2) {
							if (coma) {
								ofile << ",";
							}
							else {
								coma = true;
							}
							ofile << out.front() << ".." << out.back();
						}
						else {
							for (size_t i = 0; i < out.size(); i++) {
								if (coma) {
									ofile << ",";
								}
								else {
									coma = true;
								}
								ofile << out[i];
							}
						}
						out.clear();
						if (v < intersection.size()) {
							out.push_back(intersection[v]);
						}
					}
				}
				ofile << "])";
			}
		}

		ofile << ".";
	}
	else {
		return "ERROR: cannot open file for writing: " + output_file_name + "\n";
	}

	ofile.close();

	return "OK";
}