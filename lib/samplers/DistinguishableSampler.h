#pragma once

#include "Sampler.h"
#include "space/StateSpace.h"
#include "interferometer/Scheme.h"

class DistinguishableSampler : public Sampler {
public:
	DistinguishableSampler(StateSpace* space, Scheme& scheme, std::vector<int>& init_state);

	std::vector<float> calc_distribution(int j);
	void sample(int batch_size) override;

	StateSpace* space_ptr() override;

	std::string get_name() override;

private:
	int ph_num_, modes_num_;
	Scheme scheme_;
	StateSpace* space_ptr_;

	std::vector<int> init_state_;
};
