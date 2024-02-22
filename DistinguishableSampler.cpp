#include "DistinguishableSampler.h"

#include <random>
#include <chrono>

DistinguishableSampler::DistinguishableSampler(StateSpace* space, Scheme& scheme, std::vector<int>& init_state) :
	init_state_(init_state), scheme_(scheme), space_ptr_(space) {

	ph_num_ = (*space_ptr_).getPhNum();
	modes_num_ = (*space_ptr_).getModesNum();
}

std::vector<float> DistinguishableSampler::calc_distribution(int j) {
	std::vector<float> prob_list;
	for (int i = 0; i < modes_num_; ++i)
		prob_list.push_back(abs(scheme_.unitary_matrix[i][j]) * abs(scheme_.unitary_matrix[i][j]));

	return prob_list;
}

void DistinguishableSampler::sample(int batch_size) {
	for (int i = 0; i < batch_size; ++i) {
		std::vector<int> sample;
		std::vector<int> h;

		for (int j = 0; j < ph_num_; ++j) {
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
			std::default_random_engine generator(seed);

			std::vector<float> distribution = calc_distribution(j);
			std::discrete_distribution<> d_distr(distribution.begin(), distribution.end());

			h.push_back(d_distr(generator));
		}

		for (int j = 0; j < modes_num_; ++j) {
			int ph_num = 0;
			for (int hk : h)
				if (hk == j)
					ph_num++;
			sample.push_back(ph_num);
		}

		(*space_ptr_).AddSampledState(sample);
	}
}
