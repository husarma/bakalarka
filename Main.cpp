#include "map.hpp"

int main(int argc, char** argv) {


    Map mapa1 = Map("map.txt", "agenti.txt");
    mapa1.reload();
    mapa1.make_output("outputRoom.txt");
    map_dump(mapa1.map, "output_dump.txt");

    std::vector<std::vector<size_t>> computed_map1(mapa1.map.size(), std::vector<size_t>(mapa1.map[0].size(), 0));
    std::vector<std::vector<size_t>> computed_map2(mapa1.map.size(), std::vector<size_t>(mapa1.map[0].size(), 0));

    std::vector<std::vector<size_t>> union_map(mapa1.map.size(), std::vector<size_t>(mapa1.map[0].size(), 0));
    std::vector<std::vector<size_t>> intersect_map(mapa1.map.size(), std::vector<size_t>(mapa1.map[0].size(), 0));

    time_expanded(mapa1.map, computed_map1, mapa1.agents[0]);
    time_expanded(mapa1.map, computed_map2, mapa1.agents[1]);
    
    map_union(computed_map1, computed_map2, union_map);
    map_intersection(computed_map1, computed_map2, intersect_map);

    map_dump(union_map, "output_dump.txt");
    map_dump(intersect_map, "output_dump.txt");

    return 0;
}