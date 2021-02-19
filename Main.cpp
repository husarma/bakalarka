#include "map.hpp"

int main(int argc, char** argv) {


    Map mapa1 = Map("map.txt", "agenti.txt");
    mapa1.reload();
    mapa1.make_output("outputRoom.txt");
    map_dump(mapa1.map, "output_dump.txt");

    std::vector<std::vector<size_t>> output_map(mapa1.map.size(), std::vector<size_t>(mapa1.map[0].size(), 0));

    //time_expanded_multithread(mapa1.map, output_map, mapa1.agents);

    std::vector<std::vector<std::pair<size_t, size_t>>> output_paths(mapa1.agents.size());
    shortest_path_multithread(mapa1.map, output_paths, mapa1.agents);

    map_dump(output_map, "output_dump.txt");

    return 0;
}