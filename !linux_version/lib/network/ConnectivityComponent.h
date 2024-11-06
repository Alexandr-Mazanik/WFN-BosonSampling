#pragma once

#include "../space/FockState.h"

class ConnectivityComponent {
public:
	ConnectivityComponent() { };
	
	void AddVertex(FockState& vertex);
	float calcAVGLogProb();
	float calcLogProbDispersion();

	int getVerticesNum();

private:
	std::vector<FockState> vertices_;
	std::vector<std::vector<bool>> adj_matrix_;
};

