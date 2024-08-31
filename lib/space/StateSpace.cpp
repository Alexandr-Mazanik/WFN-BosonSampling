#include "space/StateSpace.h"
#include "math_lib.h"

#include <iostream>
#include <fstream>

StateSpace::StateSpace(int ph_num, int dim_num, bool createBasis) :
	ph_num_(ph_num), dim_num_(dim_num), fock_basis_(createBasis) {
	meanPhN_.resize(dim_num_);
	if (fock_basis_)
		CreateFockBasis();
}

StateSpace::StateSpace(const StateSpace& space) {
	fock_basis_ = space.getIsBasis();
	ph_num_ = space.getPhNum();
	dim_num_ = space.getModesNum();
	meanPhN_.resize(dim_num_);
	states = space.states;
	this->reset();
}

StateSpace::StateSpace(int dim_num, std::vector<FockState*> states_to_create) : dim_num_(dim_num) {
	ph_num_ = NULL;
	fock_basis_ = false;
	for (FockState* state : states_to_create)
		states.push_back(*state);
}

void StateSpace::CreateFockBasis() {
	int slots_num = ph_num_ + dim_num_;
	std::vector<int> indices;
	for (int i = 1; i < slots_num; ++i)
		indices.push_back(i);

	std::vector<std::vector<int>> all_comb_bars = combinations(indices, dim_num_ - 1);
	for (std::vector<int> bars : all_comb_bars) {
		bars.push_back(slots_num);
		bars.insert(bars.begin(), 0);

		std::vector<int> state;
		for (int i = 0; i < dim_num_; ++i)
			state.push_back(bars[i + 1] - bars[i] - 1);

		AddState(state);
	}
	std::cout << "--> Fock basis has been created\n";
}

void StateSpace::calcMeanPhotonNumber() {
	int points_num = 0;
	for (FockState& state : states) {
		for (int i = 0; i < dim_num_; ++i)
			meanPhN_[i] += state.getState()[i] * state.getAppearance();
		points_num += state.getAppearance();
	}
	if (points_num)
		for (int i = 0; i < dim_num_; ++i)
			meanPhN_[i] = meanPhN_[i] / (float)points_num;
}

float StateSpace::twoPointCorrFunction(int mode1, int mode2) {
	int points_num = 0;
	float cor_func = 0;
	calcMeanPhotonNumber();
	for (FockState& state : states) {
		cor_func += state.getState()[mode1] * state.getState()[mode2] * state.getAppearance();
		points_num += state.getAppearance();
	}
	if (points_num)
		cor_func /= points_num;

	return cor_func - meanPhN_[mode1] * meanPhN_[mode2];
}

float StateSpace::calcAVGLogProb() {
	float all_prob = 0.;
	std::vector<FockState*> sampled = getSampledStates();
	for (FockState* state : sampled)
		all_prob += -std::log(state->getStateProb());

	return all_prob / sampled.size();
}

float StateSpace::calcLogProbDispersion() {
	float all_prob_sq = 0.;
	std::vector<FockState*> sampled = getSampledStates();

	for (FockState* state : sampled)
		all_prob_sq += std::log(state->getStateProb()) * std::log(state->getStateProb());
	all_prob_sq /= sampled.size();
	float avg_prob = calcAVGLogProb();

	return all_prob_sq - avg_prob * avg_prob;
}

void StateSpace::ExportSampleData(std::string file_name) {
	std::ofstream file_data;
	file_data.open("../../../analysis/" + file_name);

	if (file_data.is_open()) {
		for (FockState& state : states) {
			int state_num = state.getAppearance();
			for (int i = 0; i < state_num; ++i) {
				file_data << "[";
				for (int j = 0; j < state.getState().size(); ++j) {
					if (j != state.getState().size() - 1)
						file_data << state.getState()[j] << ", ";
					else
						file_data << state.getState()[j];
				}
				file_data << "]\n";
			}
		}
		std::cout << "--> Samples has been exported\n";
	}
	else {
		std::cout << "--> Couldn't open file\n";
	}
}

void StateSpace::ImportSampleData(int sample_num, std::string file_name) {
	std::ifstream f_sample;
	std::string s_sample;

	f_sample.open("./data/" + file_name);

	if (f_sample.is_open()) {
		int i = 0;
		while (f_sample and i < sample_num) {
			std::vector<int> sample;

			std::getline(f_sample, s_sample);
			s_sample.erase(s_sample.begin());
			s_sample.erase(remove(s_sample.begin(), s_sample.end(), ','), s_sample.end());
			int j = 0;
			std::string s_ph;
			while (s_sample[j] != '\0') {
				if (s_sample[j] != ' ' and s_sample[j] != ']') {
					s_ph += s_sample[j];
				}
				else {
					sample.push_back(std::stoi(s_ph));
					s_ph.clear();
				}
				j++;
			}
			AddSampledState(sample);
			i++;
		}
		std::cout << "--> The Data file has been read\n";
	}
	else {
		std::cout << "--> Couldn't open file\n";
	}

	f_sample.close();
}

void StateSpace::AddSampledState(std::vector<int>& sample, float prob) {
	bool is_already_exist = false;
	for (int i = 0; i < states.size(); ++i)
		if (sample == states[i].getState()) {
			is_already_exist = true;
			states[i].increaseAppearance();
			break;
		}
	if (!is_already_exist) {
		states.push_back(FockState(sample, dim_num_));
		states[states.size() - 1].increaseAppearance();
		if (prob != -1)
			states[states.size() - 1].setProbability(prob);
	}
}

void StateSpace::AddState(std::vector<int>& state) {
	states.push_back(FockState(state, dim_num_));
}

void StateSpace::reset() {
	if (fock_basis_)
		for (int i = 0; i < states.size(); ++i)
			states[i].resetAppearance();
	else 
		states.clear();
}

int StateSpace::getPhNum() const {
	return ph_num_;
}

int StateSpace::getModesNum() const {
	return dim_num_;
}

bool StateSpace::getIsBasis() const {
	return fock_basis_;
}

int StateSpace::getStatesNumber() {
	return (int)states.size();
}

std::vector<FockState*> StateSpace::getSampledStates() {
	std::vector<FockState*> sampled;
	for (int i = 0; i < states.size(); ++i)
		if (states[i].getAppearance() >= 1)
			sampled.push_back(&states[i]);
	return sampled;
}

std::vector<FockState>* StateSpace::getAllStates() {
	return &states;
}

void StateSpace::printStateSpace(int points_num) {
	std::cout << "--> State space:\n";
	for (int i = 0; i < points_num; ++i) {
		std::cout << "[";
		for (int j = 0; j < dim_num_; ++j) {
			std::cout << states[i].getState()[j] << " ";
		}
		std::cout << "]\n";
		std::cout << "Node state: ";
		if (states[i].getAppearance() == 0)
			std::cout << "Empty\n";
		else
			std::cout << "Sampled\n";
		std::cout << "State prob: " << states[i].getStateProb() << std::endl;
		std::cout << "State appearance: " << states[i].getAppearance() << std::endl << std::endl;
	}
	std::cout << std::endl;
}


