#pragma once

#include "Sampler.h"
#include "../space/StateSpace.h"

class UniformSampler : public Sampler {
public:
	UniformSampler(StateSpace* space) : space_ptr_(space) { };
	
	void sample(int batch_size) override;

	StateSpace* space_ptr() override;

	std::string get_name() override;

private:
	StateSpace* space_ptr_;
};

