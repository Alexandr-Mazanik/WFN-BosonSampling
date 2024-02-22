#pragma once

#include "space/StateSpace.h"

class UniformSampler {
public:
	UniformSampler(StateSpace* space) : space_ptr_(space) { };
	
	void sample(int batch_size);

private:
	StateSpace* space_ptr_;
};

