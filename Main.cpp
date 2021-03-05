#include "map.hpp"

int main(int argc, char** argv) {


    Map mapa1 = Map("skuskam.txt", "skuskaa.txt");
    mapa1.reload();
    /*
    map_dump(mapa1.map, "output_dump.txt");

    mapa1.reset_computed_map();

    shortest_path_multiagent(mapa1.map, mapa1.agents_shortest_paths, mapa1.agents);

    paths_to_map(mapa1.agents_shortest_paths, mapa1.computed_map);

    map_dump(mapa1.computed_map, "output_dump.txt");

    expand_map(mapa1.map, mapa1.computed_map, mapa1.computed_map);

    //give_new_numbering(mapa1.computed_map);

    time_expanded_multiagent(mapa1.computed_map, mapa1.time_expanded_graph, mapa1.agents);

    mapa1.make_output("outputRoom.txt");

    auto a = are_paths_separate(mapa1.agents_shortest_paths);
    
    map_dump(mapa1.computed_map, "output_dump.txt");

    */

    mapa1.computed_map = mapa1.map;

    shortest_path_multiagent(mapa1.map, mapa1.agents_shortest_paths, mapa1.agents);
    time_expanded_multiagent(mapa1.computed_map, mapa1.time_expanded_graph, mapa1.agents, 8);

    mapa1.make_output("outputRoom.txt");



    return 0;
}