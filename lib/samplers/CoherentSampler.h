#pragma once

#include "Sampler.h"
#include "space/StateSpace.h"
#include "interferometer/Scheme.h"

class CoherentSampler : public Sampler {
public:	
	CoherentSampler(StateSpace* space, Scheme& scheme, std::vector<std::complex<float>>& init_state);

	void sample(int batch_size) override;

	StateSpace* space_ptr() override;

	std::string get_name() override;

private:
	Scheme scheme_;
	StateSpace* space_ptr_;

	std::vector<std::complex<float>> init_state_;
	std::vector<std::complex<float>> out_state_;

	int modes_num_;
};

