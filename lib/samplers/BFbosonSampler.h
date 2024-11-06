#pragma once

#include "Sampler.h"
#include "space/StateSpace.h"
#include "interferometer/Scheme.h"

class BFbosonSampler : public Sampler{
public:
	BFbosonSampler(StateSpace* space, Scheme& scheme, std::vector<int>& init_state, bool calc_distr = true);

	std::vector<std::vector<std::complex<float>>> findSubmatrix(const std::vector<int>& out_state);
	void CalculateDistribution();
	void sample(int batch_size) override;

	std::vector<int> getIndices(const std::vector<int>& state_vec);

	StateSpace* space_ptr() override;
	
	std::string get_name() override;

	void change_scheme(Scheme& scheme) override;

private:
	int ph_num_, modes_num_;
	StateSpace* space_ptr_;
	Scheme scheme_;
	std::vector<float> distribution_;
	std::vector<int> init_state_;
};

