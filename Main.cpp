#include "map.hpp"
#include "Compute_Strategy.hpp"

#include <thread>
#include <filesystem>

void runbase(std::string a, std::string b, std::string c) {

    std::unique_ptr<ComputeStrategyI> base = std::make_unique<Baseline>();

    base->set_map_file(a);
    base->set_agents_dir(b);
    base->set_output_dir(c);
    base->run_tests();
}

void runmake(std::string a, std::string b, std::string c) {

    std::unique_ptr<ComputeStrategyI> make = std::make_unique<MakespanAdd>();

    make->set_map_file(a);
    make->set_agents_dir(b);
    make->set_output_dir(c);
    make->run_tests();
}

void runprun(std::string a, std::string b, std::string c) {

    std::unique_ptr<ComputeStrategyI> prun = std::make_unique<PruningCut>();

    prun->set_map_file(a);
    prun->set_agents_dir(b);
    prun->set_output_dir(c);
    prun->run_tests();
}

void runcomb(std::string a, std::string b, std::string c) {

    std::unique_ptr<ComputeStrategyI> comb = std::make_unique<Combined>();

    comb->set_map_file(a);
    comb->set_agents_dir(b);
    comb->set_output_dir(c);
    comb->run_tests();
}

int main(int argc, char** argv) {

    std::vector<std::thread> threads;

    std::vector<std::string> dirs;

    if (argc != 3) {
        std::cout << "ERROR: Wrong number of arguments" << std::endl;
        return 1;
    }

    for (auto& p : std::filesystem::directory_iterator(argv[2])) {

        dirs.push_back(p.path().string());
    }

    for (size_t i = 0; i < dirs.size(); i++) {
        for (auto& p : std::filesystem::directory_iterator(dirs[i])) {

            std::string map_name = p.path().string();
            if (map_name.substr(map_name.size() - 4) == ".map") {
                std::cout << map_name << std::endl;

                std::string params = argv[1];
                if (params[0] != '-') {
                    std::cout << "ERROR: Wrong arguments format" << std::endl;
                    return 1;
                }
                for (size_t j = 1; j < params.size(); j++) {

                    std::string agents_dir = dirs[i] + "\\scen";

                    switch (params[j])
                    {
                    case 'b':
                        std::cout << map_name << "  " << dirs[i] + "\\scen" << "  " << dirs[i] << std::endl;
                        
                        threads.push_back(std::thread(runbase, map_name, agents_dir, dirs[i]));
                        break;
                    case 'm':
                        threads.push_back(std::thread(runmake, map_name, agents_dir, dirs[i]));
                        break;
                    case 'p':
                        threads.push_back(std::thread(runprun, map_name, agents_dir, dirs[i]));
                        break;
                    case 'c':
                        threads.push_back(std::thread(runcomb, map_name, agents_dir, dirs[i]));
                        break;
                    default:
                        std::cout << "ERROR: Undefined option: -" << params[j] << std::endl;
                        return 1;
                    }

                }

                //Join the threads with the main thread
                for (auto& thread : threads) {
                    thread.join();
                }
                threads.clear();

                std::filesystem::remove_all("temp");
            }
        }
    }
    
    return 0;
}