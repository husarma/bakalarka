#include "Debug.hpp"

std::string map_dump(std::vector<std::vector<size_t>>& reference_map, std::string dump_file_name) {

	std::ofstream ofile;
	if (dump_file_name != "") {
		ofile.open(dump_file_name, std::ios::app); //open for writing to the end of file
		if (!ofile.is_open()) {
			return "ERROR: cannot open file for writing: " + dump_file_name + "\n";
		}
	}
	else {
		//setting ofile as std::cout
		ofile.copyfmt(std::cout);
		ofile.clear(std::cout.rdstate());
		ofile.basic_ios<char>::rdbuf(std::cout.rdbuf());
	}

	ofile << std::endl;
	ofile << "Map:" << std::endl;
	for (size_t i = 0; i < reference_map.size(); i++) {
		for (size_t j = 0; j < reference_map[0].size(); j++) {
			if (reference_map[i][j] != 0) {
				ofile << ".";
				//ofile << reference_map[i][j];
			}
			else {
				ofile << "#";
			}
		}
		ofile << std::endl;
	}

	if (dump_file_name != "") {
		ofile.close();
	}

	return "OK";
}
