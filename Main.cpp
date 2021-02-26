#include "map.hpp"

int main(int argc, char** argv) {


    Map mapa1 = Map("map.txt", "agenti.txt");
    mapa1.reload();
    mapa1.make_output_without_preprocesing("outputRoom.txt", mapa1.map);
    map_dump(mapa1.map, "output_dump.txt");

    mapa1.reset_computed_map();

    shortest_path_multiagent(mapa1.map, mapa1.agents_shortest_paths, mapa1.agents);

    paths_to_map(mapa1.agents_shortest_paths, mapa1.computed_map);

    map_dump(mapa1.computed_map, "output_dump.txt");

    expand_map(mapa1.map, mapa1.computed_map, mapa1.computed_map);

    //give_new_numbering(mapa1.computed_map);

    time_expanded(mapa1.computed_map, mapa1.time_expanded_graph, 0, mapa1.agents[0]);

    auto a = are_paths_separate(mapa1.agents_shortest_paths);
    
    map_dump(mapa1.computed_map, "output_dump.txt");


    return 0;
}