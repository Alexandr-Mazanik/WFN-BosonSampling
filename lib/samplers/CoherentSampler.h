#pragma once

#include "space/StateSpace.h"
#include "interferometer/Scheme.h"

class CoherentSampler {
public:	
	CoherentSampler(StateSpace* space, Scheme& scheme, std::vector<std::complex<float>>& init_state);

	void sample(int batch_size);

private:
	StateSpace* space_ptr_;
	Scheme scheme_;

	std::vector<std::complex<float>> init_state_;
	std::vector<std::complex<float>> out_state_;

	int modes_num_;
};

