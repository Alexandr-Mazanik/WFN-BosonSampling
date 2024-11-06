#include "../samplers/MISampler.h"
#include "../math_lib.h"
#include "../../bs-StateSpace.h"

#include <iostream>
#include <cfloat>

MISampler::MISampler(StateSpace* space, Scheme& scheme, std::vector<int>& init_state,
	       	int tau_burn, int tau_thin, bool autocorr_reduction) :
	init_state_(init_state), scheme_(scheme), space_ptr_(space), 
	ph_num_(space->getPhNum()), modes_num_(space->getModesNum()), curr_mc_state_(init_state_, modes_num_),
	base_space_(ph_num_, modes_num_, false), base_sampler_(&base_space_, scheme, init_state),
	autocorr_reduction_(autocorr_reduction) { 

	tau_burn_ = tau_burn;
	tau_thin_ = tau_thin;

	reset_mc();
}

float MISampler::CalculateStateProb(FockState* state) {
	int norm = 1;
	for (int num : init_state_)
		norm *= factorial(num);
	for (int num : state->getState())
		norm *= factorial(num);

	float perm_abs = abs(perm(findSubmatrix(state->getState()), ph_num_));

	return perm_abs * perm_abs / (float)norm;

}

std::vector<std::vector<std::complex<float>>> MISampler::findSubmatrix(const std::vector<int>& out_state) {
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

std::vector<int> MISampler::getIndices(const std::vector<int>& state_vec) {
	std::vector<int> indices;
	for (int i = 0; i < modes_num_; ++i) {
		if (state_vec[i] > 0)
			for (int j = 0; j < state_vec[i]; ++j)
				indices.push_back(i);
	}

	return indices;
}

void MISampler::MC_step() {
	
	//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	//srand(seed);
	std::uniform_real_distribution<double> dist(0, std::nextafter(1, DBL_MAX));

	mc_step_num_++;

	base_sampler_.sample(1);
	FockState try_step = *(base_space_.getSampledStates()[0]);
	base_space_.reset();

	float accept_prob = (CalculateStateProb(&try_step) * 
		base_sampler_.CalculateStateProb(&curr_mc_state_) /
		(CalculateStateProb(&curr_mc_state_) *
			base_sampler_.CalculateStateProb(&try_step)));
	if (accept_prob > 1)
		accept_prob = 1;

	if (dist(generator) <= accept_prob)
		curr_mc_state_ = try_step;
}

void MISampler::sample(int batch_size) {
	int samples_num = 0;

	while (samples_num < batch_size) {
		MC_step();

		if (mc_step_num_ >= tau_burn_ and (mc_step_num_ - tau_burn_) % tau_thin_ == 0) {
			std::vector<int> state = curr_mc_state_.getState();
			int states_num = space_ptr_->getStatesNumber();

			space_ptr_->AddSampledState(state);
			
			if (space_ptr_->getStatesNumber() != states_num and autocorr_reduction_)
				samples_num++;
			if (!autocorr_reduction_)
				samples_num++;
		}

	}
}

void MISampler::reset_mc() {
	mc_step_num_ = 0;

	base_sampler_.sample(1);
	curr_mc_state_ = *(base_space_.getSampledStates()[0]);
	base_space_.reset();
}

StateSpace* MISampler::space_ptr() {
	return space_ptr_;
}

std::string MISampler::get_name() {
	return "mis";
}
