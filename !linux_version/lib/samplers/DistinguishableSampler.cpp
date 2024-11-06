#include "../samplers/DistinguishableSampler.h"
#include "../math_lib.h"
#include "../../bs-StateSpace.h"

#include <iostream>

DistinguishableSampler::DistinguishableSampler(StateSpace* space, Scheme& scheme, std::vector<int>& init_state) :
	init_state_(init_state), scheme_(scheme), space_ptr_(space) {

	ph_num_ = (*space_ptr_).getPhNum();
	modes_num_ = (*space_ptr_).getModesNum();
}

std::vector<int> DistinguishableSampler::getIndices(const std::vector<int>& state_vec) {
	std::vector<int> indices;
	for (int i = 0; i < modes_num_; ++i) {
		if (state_vec[i] > 0)
			for (int j = 0; j < state_vec[i]; ++j)
				indices.push_back(i);
	}

	return indices;
}

std::vector<std::vector<std::complex<float>>> DistinguishableSampler::findSubmatrix(const std::vector<int>& out_state) {
	std::vector<int> column_indices = getIndices(init_state_);
	std::vector<int> row_indices = getIndices(out_state);

	std::vector<std::vector<std::complex<float>>> submatrix;
	std::vector<std::vector<std::complex<float>>> temp_mat(modes_num_);

	for (int col_ind : column_indices)
		for (int i = 0; i < modes_num_; ++i)
			temp_mat[i].push_back(scheme_.unitary_matrix[i][col_ind]);

	for (int row_ind : row_indices)
		submatrix.push_back(temp_mat[row_ind]);

	return submatrix;
}

float DistinguishableSampler::CalculateStateProb(FockState* state) {
	int norm = 1;
	for (int num : init_state_)
		norm *= factorial(num);
	for (int num : state->getState())
		norm *= factorial(num);

	std::vector<std::vector<std::complex<float>>> abs_sub = abs_mat(findSubmatrix(state->getState()));
	std::complex<float> permanent = perm(mat_mul(abs_sub, abs_sub), ph_num_);

	return permanent.real() / (float)norm;
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
			//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
			//std::default_random_engine generator(seed);

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

StateSpace* DistinguishableSampler::space_ptr() {
	return space_ptr_;
}

void DistinguishableSampler::change_scheme(Scheme& scheme) {
	scheme_ = scheme;
}

std::string DistinguishableSampler::get_name() {
	return "ds";
}
