#pragma once

#include "Debug.hpp"
#include "Algorithms.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

/**
 *  Class that holds information about map and agents.
 *  Can generate input for picat.
 */
class Map {
public:
    const size_t height_number_index = 7;
    const size_t width_number_index = 6;

    std::string map_file_name = "";
    std::string agents_file_name = "";

    size_t height = 0;
    size_t width = 0;

    std::vector<std::vector<size_t>> map;
    std::vector<std::vector<size_t>> computed_map;
    //agent is defined by coordinates of his start pint and finish point
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> agents;

    std::vector<std::vector<std::pair<size_t, size_t>>> agents_shortest_paths;

    //   agent       time        new vertices added in this time
    std::vector<std::vector<std::vector<size_t>>> time_expanded_graph;

    Map();
    Map(std::string map_file_name, std::string agents_file_name);

    void set_map_file(std::string new_map_file_name);
    void set_agents_file(std::string new_agents_file_name);

    std::string load_map(std::string custom_map_file_name = "");
    std::string load_agents(std::string custom_agents_file_name = "");
    std::string reload();
    std::string make_output_without_preprocesing(std::string output_file_name, std::vector<std::vector<size_t>>& map);
    
    void reset_computed_map();
};
