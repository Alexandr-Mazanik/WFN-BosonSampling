#pragma once
#pragma once

#include "Sampler.h"
#include "space/StateSpace.h"
#include "interferometer/Scheme.h"

class MeanFieldSampler : public Sampler {
public:
	MeanFieldSampler(StateSpace* space, Scheme& scheme, std::vector<int>& init_state);

	std::vector<float> calc_distribution(std::vector<std::complex<float>>& theta);
	void sample(int batch_size) override;

	StateSpace* space_ptr() override;

	std::string get_name() override;

	void change_scheme(Scheme& scheme) override;

private:
	int ph_num_, modes_num_;
	Scheme scheme_;
	StateSpace* space_ptr_;

	std::vector<int> init_state_;
};