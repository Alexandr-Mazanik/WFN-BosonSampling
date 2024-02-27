#pragma once

#include "space/StateSpace.h"

class Network {
public:
	Network(StateSpace* space_ptr, int radius);
	Network(StateSpace* space_ptr);
	Network(const Network& network);

	void BuildNetwork(int radius);

	void countConnectedComponents();
	void DFS(int vertex, std::vector<bool>& visited);
	std::vector<int> componentsDistribution();
	
	std::vector<float> degreeDistribution();
	std::vector<float> degDistTwoMoments(const std::vector<float>& distr);

	std::vector<Network>& getConComp();

	void printNetwork();

private:
	StateSpace* networkSpace_;

	std::vector<Network> con_comp_;
	std::vector<std::vector<bool>> network_;

	int vertices_num_;
};

