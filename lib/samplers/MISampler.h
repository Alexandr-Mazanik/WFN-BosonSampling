#pragma once

#include "Sampler.h"
#include "space/StateSpace.h"
#include "interferometer/Scheme.h"
#include "samplers/DistinguishableSampler.h"

class MISampler : public Sampler {
public:
	MISampler(StateSpace* space, Scheme& scheme, std::vector<int>& init_state, 
		int tau_burn, int tau_thin);

	float CalculateStateProb(FockState* state);
	void MC_step();
	void sample(int batch_size) override;

	std::vector<std::vector<std::complex<float>>> findSubmatrix(const std::vector<int>& out_state);
	std::vector<int> getIndices(const std::vector<int>& state_vec);

	void reset_mc();

	StateSpace* space_ptr() override;

	std::string get_name() override;

private:
	int ph_num_, modes_num_;
	int tau_burn_, tau_thin_;
	int mc_step_num_;
	
	Scheme scheme_;
	StateSpace* space_ptr_;

	std::vector<int> init_state_;
	StateSpace base_space_;
	FockState curr_mc_state_;
	DistinguishableSampler base_sampler_;
};

