#include "Compute_Strategy.hpp"

#include <filesystem>
#include <chrono>

/** Setting path to map.
*
* @param path file containing map.
*/
void ComputeStrategyI::set_map_file(std::string path) {
	map_path = path;
}

/** Setting path to agents.
*
* @param path directory containing agents.
*/
void ComputeStrategyI::set_agents_dir(std::string path) {
	agents_dir_path = path;
}

/** Setting path to output directory.
*
* @param path directory for generating output to.
*/
void ComputeStrategyI::set_output_dir(std::string path) {
	output_dir_path = path;
}

/** Execute concrete computing algorithm for finding solution.*/
std::string Baseline::run_tests() {

	std::vector<std::string> agents_paths;

	for (const auto& entry : std::filesystem::directory_iterator(agents_dir_path)) {
		agents_paths.push_back(entry.path().string());
	}

	Map mapa;
	mapa.set_outputter(std::make_unique<OPreprocessFlatten1>());
	mapa.set_map_file(map_path);
	std::string err = mapa.load_map();
	if (err != "OK") {
		std::cout << err << std::endl;
		return err;
	}
	mapa.computed_map = mapa.map;

	for (size_t i = 0; i < agents_paths.size(); i++) {

		int number_of_agents_to_compute = 0;
		size_t LB = 0;
		size_t bonus_makespan = 0;
		mapa.set_agents_file(agents_paths[i]);
		std::string result = "OK";

		std::chrono::steady_clock::time_point started;

		while (result != "Timed out") {

			if (result == "OK") {

				started = std::chrono::high_resolution_clock::now();

				bonus_makespan = 0;
				number_of_agents_to_compute += 5;

				err = mapa.load_agents(number_of_agents_to_compute);
				if (err != "OK") {
					std::cout << err << std::endl;
					return err;
				}

				err = shortest_path_multiagent(mapa.map, mapa.agents_shortest_paths, mapa.agents);
				if (err != "OK") {
					std::cout << err << std::endl;
					return err;
				}

				LB = mapa.get_min_time();
			}

			if (result == "NO solution") {
				bonus_makespan++;
			}

			err = time_expanded_multiagent(mapa.computed_map, mapa.time_expanded_graph, mapa.agents, LB + bonus_makespan);
			if (err != "OK") {
				std::cout << err << std::endl;
				return err;
			}

			auto computed_result = mapa.picat(output_dir_path + "\\B_log.txt", "B"); //if "OK" add agents, if "NO solution" add makespan, if "Timed out" end
			result = computed_result.first;
			if (result == "OK") {

				auto done = std::chrono::high_resolution_clock::now();
				long long elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count();
				std::ofstream ofile;
				ofile.open(output_dir_path + "\\B_results.txt", std::ios::app);

				if (ofile.is_open()) {
					auto [map_file, agents_file, number_of_vertices, number_of_agents] = computed_result.second;

					std::string table_row =
						map_file
						+ "\t" +
						agents_file
						+ "\t" +
						std::to_string(number_of_vertices)
						+ "\t" +
						std::to_string(number_of_agents)
						+ "\t" +
						std::to_string(LB)
						+ "\t" +
						std::to_string(LB + bonus_makespan)
						+ "\t" +
						std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count());
					ofile << table_row << std::endl;
				}

				ofile.close();
			}
		}
	}

	return "OK";
}

/** Execute concrete computing algorithm for finding solution.*/
std::string MakespanAdd::run_tests() {

	std::vector<std::string> agents_paths;

	for (const auto& entry : std::filesystem::directory_iterator(agents_dir_path)) {
		agents_paths.push_back(entry.path().string());
	}

	Map mapa;
	mapa.set_outputter(std::make_unique<OPreprocessFlatten1>());
	mapa.set_map_file(map_path);
	std::string err = mapa.load_map();
	if (err != "OK") {
		std::cout << err << std::endl;
		return err;
	}
	//mapa.computed_map = mapa.map;

	for (size_t i = 0; i < agents_paths.size(); i++) {

		int number_of_agents_to_compute = 0;
		size_t LB = 0;
		size_t bonus_makespan = 0;
		mapa.set_agents_file(agents_paths[i]);
		std::string result = "OK";

		std::chrono::steady_clock::time_point started;

		while (result != "Timed out") {

			if (result == "OK") {

				started = std::chrono::high_resolution_clock::now();

				bonus_makespan = 0;
				number_of_agents_to_compute += 5;

				err = mapa.load_agents(number_of_agents_to_compute);
				if (err != "OK") {
					std::cout << err << std::endl;
					return err;
				}

				err = shortest_path_multiagent(mapa.map, mapa.agents_shortest_paths, mapa.agents);
				if (err != "OK") {
					std::cout << err << std::endl;
					return err;
				}
				mapa.reset_computed_map();
				paths_to_map(mapa.agents_shortest_paths, mapa.computed_map);
				err = expand_map(mapa.map, mapa.computed_map, mapa.computed_map).first;
				if (err != "OK") {
					std::cout << err << std::endl;
					return err;
				}

				LB = mapa.get_min_time();
			}

			if (result == "NO solution") {
				bonus_makespan++;
			}

			err = time_expanded_multiagent(mapa.computed_map, mapa.time_expanded_graph, mapa.agents, LB + bonus_makespan);
			if (err != "OK") {
				std::cout << err << std::endl;
				return err;
			}

			auto computed_result = mapa.picat(output_dir_path + "\\M_log.txt", "M"); //if "OK" add agents, if "NO solution" add makespan, if "Timed out" end
			result = computed_result.first;
			if (result == "OK") {

				auto done = std::chrono::high_resolution_clock::now();
				long long elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count();
				std::ofstream ofile;
				ofile.open(output_dir_path + "\\M_results.txt", std::ios::app);

				if (ofile.is_open()) {
					auto [map_file, agents_file, number_of_vertices, number_of_agents] = computed_result.second;

					std::string table_row =
						map_file
						+ "\t" +
						agents_file
						+ "\t" +
						std::to_string(number_of_vertices)
						+ "\t" +
						std::to_string(number_of_agents)
						+ "\t" +
						std::to_string(LB)
						+ "\t" +
						std::to_string(LB + bonus_makespan)
						+ "\t" +
						std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count());
					ofile << table_row << std::endl;
				}

				ofile.close();
			}
		}
	}

	return "OK";
}

/** Execute concrete computing algorithm for finding solution.*/
std::string PruningCut::run_tests() {

	std::vector<std::string> agents_paths;

	for (const auto& entry : std::filesystem::directory_iterator(agents_dir_path)) {
		agents_paths.push_back(entry.path().string());
	}

	Map mapa;
	mapa.set_outputter(std::make_unique<OPreprocessFlatten1>());
	mapa.set_map_file(map_path);
	std::string err = mapa.load_map();
	if (err != "OK") {
		std::cout << err << std::endl;
		return err;
	}
	//mapa.computed_map = mapa.map;

	for (size_t i = 0; i < agents_paths.size(); i++) {

		int number_of_agents_to_compute = 0;
		size_t LB = 0;
		size_t bonus_makespan = 0;
		mapa.set_agents_file(agents_paths[i]);
		std::string result = "OK";

		std::chrono::steady_clock::time_point started;

		while (result != "Timed out") {

			if (result == "OK") {

				started = std::chrono::high_resolution_clock::now();

				bonus_makespan = 0;
				number_of_agents_to_compute += 5;

				err = mapa.load_agents(number_of_agents_to_compute);
				if (err != "OK") {
					std::cout << err << std::endl;
					return err;
				}

				err = shortest_path_multiagent(mapa.map, mapa.agents_shortest_paths, mapa.agents);
				if (err != "OK") {
					std::cout << err << std::endl;
					return err;
				}
				mapa.reset_computed_map();
				paths_to_map(mapa.agents_shortest_paths, mapa.computed_map);
				
				LB = mapa.get_min_time();
			}

			if (result == "NO solution") {
				auto res = expand_map(mapa.map, mapa.computed_map, mapa.computed_map); 
				err = res.first; 
				if (err != "OK") {
					std::cout << err << std::endl;
					return err;
				}
				if (!res.second) { //if not expanded then add makespan and make max pruning
					bonus_makespan++;
					mapa.reset_computed_map();
					paths_to_map(mapa.agents_shortest_paths, mapa.computed_map);
				}
			}

			err = time_expanded_multiagent(mapa.computed_map, mapa.time_expanded_graph, mapa.agents, LB + bonus_makespan);
			if (err != "OK") {
				std::cout << err << std::endl;
				return err;
			}

			auto computed_result = mapa.picat(output_dir_path + "\\P_log.txt", "P"); //if "OK" add agents, if "NO solution" add vertices, if "Timed out" end
			result = computed_result.first;
			if (result == "OK") {

				auto done = std::chrono::high_resolution_clock::now();
				long long elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count();
				std::ofstream ofile;
				ofile.open(output_dir_path + "\\P_results.txt", std::ios::app);

				if (ofile.is_open()) {
					auto [map_file, agents_file, number_of_vertices, number_of_agents] = computed_result.second;

					std::string table_row =
						map_file
						+ "\t" +
						agents_file
						+ "\t" +
						std::to_string(number_of_vertices)
						+ "\t" +
						std::to_string(number_of_agents)
						+ "\t" +
						std::to_string(LB)
						+ "\t" +
						std::to_string(LB + bonus_makespan)
						+ "\t" +
						std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count());
					ofile << table_row << std::endl;
				}

				ofile.close();
			}
		}
	}

	return "OK";
}

/** Execute concrete computing algorithm for finding solution.*/
std::string Combined::run_tests() {

	std::vector<std::string> agents_paths;

	for (const auto& entry : std::filesystem::directory_iterator(agents_dir_path)) {
		agents_paths.push_back(entry.path().string());
	}

	Map mapa;
	mapa.set_outputter(std::make_unique<OPreprocessFlatten1>());
	mapa.set_map_file(map_path);
	std::string err = mapa.load_map();
	if (err != "OK") {
		std::cout << err << std::endl;
		return err;
	}
	//mapa.computed_map = mapa.map;

	for (size_t i = 0; i < agents_paths.size(); i++) {

		int number_of_agents_to_compute = 0;
		size_t LB = 0;
		size_t bonus_makespan = 0;
		mapa.set_agents_file(agents_paths[i]);
		std::string result = "OK";

		std::chrono::steady_clock::time_point started;

		while (result != "Timed out") {

			if (result == "OK") {

				started = std::chrono::high_resolution_clock::now();

				bonus_makespan = 0;
				number_of_agents_to_compute += 5;

				err = mapa.load_agents(number_of_agents_to_compute);
				if (err != "OK") {
					std::cout << err << std::endl;
					return err;
				}

				err = shortest_path_multiagent(mapa.map, mapa.agents_shortest_paths, mapa.agents);
				if (err != "OK") {
					std::cout << err << std::endl;
					return err;
				}
				mapa.reset_computed_map();
				paths_to_map(mapa.agents_shortest_paths, mapa.computed_map);

				LB = mapa.get_min_time();
			}

			if (result == "NO solution") {
				bonus_makespan++;
				err = expand_map(mapa.map, mapa.computed_map, mapa.computed_map).first;
				if (err != "OK") {
					std::cout << err << std::endl;
					return err;
				}
			}

			err = time_expanded_multiagent(mapa.computed_map, mapa.time_expanded_graph, mapa.agents, LB + bonus_makespan);
			if (err != "OK") {
				std::cout << err << std::endl;
				return err;
			}

			auto computed_result = mapa.picat(output_dir_path + "\\C_log.txt", "C"); //if "OK" add agents, if "NO solution" add vertices and makespan, if "Timed out" end
			result = computed_result.first;
			if (result == "OK") {

				auto done = std::chrono::high_resolution_clock::now();
				long long elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count();
				std::ofstream ofile;
				ofile.open(output_dir_path + "\\C_results.txt", std::ios::app);

				if (ofile.is_open()) {
					auto [map_file, agents_file, number_of_vertices, number_of_agents] = computed_result.second;

					std::string table_row =
						map_file
						+ "\t" +
						agents_file
						+ "\t" +
						std::to_string(number_of_vertices)
						+ "\t" +
						std::to_string(number_of_agents)
						+ "\t" +
						std::to_string(LB)
						+ "\t" +
						std::to_string(LB + bonus_makespan)
						+ "\t" +
						std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count());
					ofile << table_row << std::endl;
				}

				ofile.close();
			}
		}
	}

	return "OK";
}