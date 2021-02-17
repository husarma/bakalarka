#include "map.hpp"

int main(int argc, char** argv) {
    Map mapa = Map(argv[1], argv[2]);
    mapa.load_map_and_agents();
    mapa.make_output("output2.txt");
    return 0;
}