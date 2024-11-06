#pragma once

#include "../space/StateSpace.h"
#include "../interferometer/Scheme.h"

class Sampler {
public:
	virtual void sample(int batch_size) = 0;

	virtual StateSpace* space_ptr() = 0;

	virtual std::string get_name() = 0;

	virtual void change_scheme(Scheme& scheme) = 0;

};

