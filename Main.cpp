#include "map.hpp"

int main(int argc, char** argv) {


    Map mapa1 = Map(argv[1], argv[2]);
    mapa1.reload();
    mapa1.make_output("output1.txt");
    mapa1.map_dump("output_dump.txt");


    Map mapa2 = Map(argv[1], argv[2]);
    mapa2.load_map();
    mapa2.load_agents();
    mapa2.make_output("output2.txt");


    Map mapa3 = Map();
    mapa3.load_map(argv[1]);
    mapa3.load_agents(argv[2]);
    mapa3.make_output("output3.txt");


    Map mapa4 = Map();
    mapa4.set_map_file(argv[1]);
    mapa4.set_agents_file(argv[2]);
    mapa4.load_map();
    mapa4.load_agents();
    mapa4.make_output("output4.txt");
    return 0;
}