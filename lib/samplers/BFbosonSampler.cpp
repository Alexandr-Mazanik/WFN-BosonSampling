#include "samplers/BFbosonSampler.h"
#include "math_lib.h"
#include "../bs-StateSpace.h"

#include <iostream>

BFbosonSampler::BFbosonSampler(StateSpace* space, Scheme& scheme, std::vector<int>& init_state, bool calc_distr) :
	init_state_(init_state), scheme_(scheme), space_ptr_(space) {
	ph_num_ = (*space_ptr_).getPhNum();
	modes_num_ = (*space_ptr_).getModesNum();

	if (calc_distr)
		CalculateDistribution();
}

std::vector<std::vector<std::complex<float>>> BFbosonSampler::findSubmatrix(const std::vector<int>& out_state) {
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

void BFbosonSampler::CalculateDistribution() {
	for (int i = 0; i < space_ptr_->getAllStates()->size(); ++i) {
		int norm = 1;
		for (int num : init_state_)
			norm *= factorial(num);
		for (int num : (*space_ptr_->getAllStates())[i].getState())
			norm *= factorial(num);

		float perm_abs = abs(perm(findSubmatrix((*space_ptr_->getAllStates())[i].getState()), ph_num_));

		float prob = perm_abs * perm_abs / (float)norm;
		(*space_ptr_->getAllStates())[i].setProbability(prob);

		distribution_.push_back(prob);
	}
	std::cout << "--> Distribution has been calculated\n";
}

void BFbosonSampler::sample(int batch_size) {
	//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	//std::default_random_engine generator(seed);

	std::discrete_distribution<> d_distr(distribution_.begin(), distribution_.end());
	for (int i = 0; i < batch_size; ++i) {
		int sample_num = d_distr(generator);
		space_ptr_->states[sample_num].increaseAppearance();
	}
}

std::vector<int> BFbosonSampler::getIndices(const std::vector<int>& state_vec) {
	std::vector<int> indices;
	for (int i = 0; i < modes_num_; ++i) {
		if (state_vec[i] > 0)
			for (int j = 0; j < state_vec[i]; ++j)
				indices.push_back(i);
	}

	return indices;
}

StateSpace* BFbosonSampler::space_ptr() {
	return space_ptr_;
}

std::string BFbosonSampler::get_name() {
	return "bf";
}

void BFbosonSampler::change_scheme(Scheme& scheme) {
	scheme_ = scheme;
	distribution_.clear();
	CalculateDistribution();
}
