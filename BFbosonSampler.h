#pragma once

#include "StateSpace.h"
#include "Scheme.h"

class BFbosonSampler {
public:
	BFbosonSampler(StateSpace* space, Scheme& scheme, std::vector<int>& init_state);

	std::vector<std::vector<std::complex<float>>> findSubmatrix(const std::vector<int>& out_state);
	void CalculateDistribution();
	void sample(int batch_size);

	std::vector<int> getIndices(const std::vector<int>& state_vec);

private:
	int ph_num_, modes_num_;
	StateSpace* space_ptr_;
	Scheme scheme_;
	std::vector<float> distribution_;
	std::vector<int> init_state_;
};

