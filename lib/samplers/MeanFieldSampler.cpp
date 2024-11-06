#define _USE_MATH_DEFINES

#include "samplers/MeanFieldSampler.h"
#include "../bs-StateSpace.h"

#include <cstdlib>
#include <stdexcept>

MeanFieldSampler::MeanFieldSampler(StateSpace* space, Scheme& scheme, std::vector<int>& init_state) :
	scheme_(scheme), space_ptr_(space) {

	ph_num_ = (*space_ptr_).getPhNum();
	modes_num_ = (*space_ptr_).getModesNum();

	for (int i = 0; i < modes_num_; ++i) 
		if (init_state[i] != 0) {
			if (init_state[i] > 1)
				throw std::invalid_argument("The mean-field sampler requires single photons at the input modes.");
			init_state_.push_back(i);
				
		}
}

std::vector<float> MeanFieldSampler::calc_distribution(std::vector<std::complex<float>>& theta) {
	std::vector<float> prob_list;

	for (int j = 0; j < modes_num_; ++j) {
		std::complex<float> ampl = 0;
		for (int k = 0; k < ph_num_; ++k)
			ampl += exp(theta[k]) * scheme_.unitary_matrix[init_state_[k]][j];

		prob_list.push_back(abs(ampl) * abs(ampl) / ph_num_);
	}

	return prob_list;
}

void MeanFieldSampler::sample(int batch_size) {
	for (int i = 0; i < batch_size; ++i) {
		//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		//srand(seed);

		std::uniform_real_distribution<double> dist(0, std::nextafter(1, DBL_MAX));
		std::vector<int> sample(modes_num_, 0);
		std::vector<std::complex<float>> theta;
		for (int k = 0; k < ph_num_; ++k)
			theta.push_back(std::complex<float>(0.0, 2 * M_PI * dist(generator)));

		//std::default_random_engine generator(seed);

		std::vector<float> distribution = calc_distribution(theta);
		std::discrete_distribution<> d_distr(distribution.begin(), distribution.end());

		for (int k = 0; k < ph_num_; ++k)
			sample[d_distr(generator)] += 1;
		
		(*space_ptr_).AddSampledState(sample);
	}
}

StateSpace* MeanFieldSampler::space_ptr() {
	return space_ptr_;
}

std::string MeanFieldSampler::get_name() {
	return "mf";
}

void MeanFieldSampler::change_scheme(Scheme& scheme) {
	scheme_ = scheme;
}
