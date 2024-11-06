#pragma once

#include "Sampler.h"
#include "../space/StateSpace.h"
#include "../interferometer/Scheme.h"

class DistinguishableSampler : public Sampler {
public:
	DistinguishableSampler(StateSpace* space, Scheme& scheme, std::vector<int>& init_state);

	std::vector<float> calc_distribution(int j);
	void sample(int batch_size) override;
	
	std::vector<std::vector<std::complex<float>>> findSubmatrix(const std::vector<int>& out_state);
	float CalculateStateProb(FockState* state);
	std::vector<int> getIndices(const std::vector<int>& state_vec);

	StateSpace* space_ptr() override;

	std::string get_name() override;

	void change_scheme(Scheme& scheme) override;

private:
	int ph_num_, modes_num_;
	Scheme scheme_;
	StateSpace* space_ptr_;

	std::vector<int> init_state_;
};
