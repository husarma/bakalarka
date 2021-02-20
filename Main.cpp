#include "map.hpp"

int main(int argc, char** argv) {


    Map mapa1 = Map("map.txt", "agenti.txt");
    mapa1.reload();
    mapa1.make_output("outputRoom.txt");
    map_dump(mapa1.referenece_map, "output_dump.txt");

    std::vector<std::vector<size_t>> computed_map1(mapa1.referenece_map.size(), std::vector<size_t>(mapa1.referenece_map[0].size(), 0));

    shortest_path_multithread(mapa1.referenece_map, mapa1.agents_shortest_paths, mapa1.agents);

    paths_to_map(mapa1.agents_shortest_paths, computed_map1);

    map_dump(computed_map1, "output_dump.txt");

    add_free_surroundings(mapa1.referenece_map, computed_map1, computed_map1);
    
    map_dump(computed_map1, "output_dump.txt");


    return 0;
}