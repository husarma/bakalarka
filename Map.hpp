#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


class Map {
public:
    const size_t height_number_index = 7;
    const size_t width_number_index = 6;

    std::string map_file_name;
    std::string agents_file_name;

    std::vector<std::vector<size_t>> graph;

    Map();
    Map(std::string map_file_name, std::string agents_file_name);

    void set_map_file(std::string new_map_file_name);
    void set_agents_file(std::string new_agents_file_name);
    int load_map();
};
