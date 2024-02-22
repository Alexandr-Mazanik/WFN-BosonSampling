#pragma once

#include "Scheme.h"
#include "StateSpace.h"

class DistinguishableSampler {
public:
	DistinguishableSampler(StateSpace* space, Scheme& scheme, std::vector<int>& init_state);

	std::vector<float> calc_distribution(int j);
	void sample(int batch_size);

private:
	int ph_num_, modes_num_;
	StateSpace* space_ptr_;
	Scheme scheme_;

	std::vector<int> init_state_;
};
