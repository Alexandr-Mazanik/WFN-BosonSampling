#include "CoherentSampler.h"
#include "math.h"

#include <random>
#include <chrono>
#include <cmath>
#include <iostream>

CoherentSampler::CoherentSampler(StateSpace* space, Scheme& scheme, std::vector<std::complex<float>> & init_state) :
	init_state_(init_state), scheme_(scheme), space_ptr_(space) {
	
	std::vector<std::vector<std::complex<float>>> init_st_m;
	std::vector<std::vector<std::complex<float>>> out_st_m;

	for (std::complex<float> alpha: init_state_) {
		std::vector<std::complex<float>> alpha_v = {alpha};
		init_st_m.push_back(alpha_v);
	}

	out_st_m = mat_mul(transpose(scheme_.unitary_matrix), init_st_m);

	for (std::vector<std::complex<float>> beta_v : out_st_m)
		out_state_.push_back(beta_v[0]);
	
	modes_num_ = (*space_ptr_).getModesNum();
}

void CoherentSampler::sample(int batch_size) {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	std::vector<std::poisson_distribution<int>> out_dist;
	for (int m = 0; m < modes_num_; ++m) {
		float mean = abs(out_state_[m]) * abs(out_state_[m]);
		if (mean == 0)
			mean = 0.0000001;
		std::poisson_distribution<int> dist(mean);
		out_dist.push_back(dist);
	}

	for (int i = 0; i < batch_size; ++i) {
		std::vector<int> sample;
		for (int m = 0; m < modes_num_; ++m) 
			sample.push_back(out_dist[m](generator));

		float prob = 1;
		for (int i = 0; i < modes_num_; ++i) {
			float k = abs(out_state_[i]) * abs(out_state_[i]);
			prob *= std::exp(-k) * pow(k, sample[i]) / factorial(k);
		}

		(*space_ptr_).AddSampledState(sample, prob);
	}
}
