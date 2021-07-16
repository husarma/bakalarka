#pragma once

#include "Map.hpp"

/**
* Strategy design pattern.
* Interface for main solution search strategy.
*/
class ComputeStrategyI {
protected:
	std::string map_path;
	std::string agents_dir_path;
	std::string output_dir_path;
public:
	void set_map_file(std::string path);
	void set_agents_dir(std::string path);
	void set_output_dir(std::string path);

	virtual ~ComputeStrategyI() {}
	virtual std::string run_tests() = 0;
};

/** Concrete implementation of search strategy for finding an solution.*/
class Baseline : public ComputeStrategyI {
	std::string run_tests() override;
};

/** Concrete implementation of search strategy for finding an solution.*/
class MakespanAdd : public ComputeStrategyI {
	std::string run_tests() override;
};

/** Concrete implementation of search strategy for finding an solution.*/
class PruningCut : public ComputeStrategyI {
	std::string run_tests() override;
};

/** Concrete implementation of search strategy for finding an solution.*/
class Combined : public ComputeStrategyI {
	std::string run_tests() override;
};