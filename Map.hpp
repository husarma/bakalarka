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

    size_t height = 0;
    size_t width = 0;

    std::vector<std::vector<size_t>> map;
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> agents;

    Map();
    Map(std::string map_file_name, std::string agents_file_name);

    void set_map_file(std::string new_map_file_name);
    void set_agents_file(std::string new_agents_file_name);
    int load_map_and_agents();
    int make_output(std::string output_file_name);
};
