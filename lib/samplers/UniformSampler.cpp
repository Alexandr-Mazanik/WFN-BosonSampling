#include "samplers/UniformSampler.h"
#include "../bs-StateSpace.h"

#include <iostream>

void UniformSampler::sample(int batch_size) {

	std::uniform_int_distribution<int> d_distr(0, space_ptr_->getAllStates()->size() - 1);
	for (int i = 0; i < batch_size; ++i) {
		int sample_num = d_distr(generator);
		space_ptr_->states[sample_num].increaseAppearance();
	}
}

StateSpace* UniformSampler::space_ptr() {
	return space_ptr_;
}

std::string UniformSampler::get_name() {
	return "uf";
}
