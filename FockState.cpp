#include "FockState.h"
#include <iostream>

FockState::FockState(std::vector<int>& state, int dim_num) :
	state_(state), dim_num_(dim_num) {
	appearance_ = 0;
	probability_ = -1;
}

int FockState::L1Distance(const FockState& other) {
	int dist = 0;
	for (int i = 0; i < dim_num_; ++i) {
		dist += abs(this->state_[i] - other.state_[i]);
	}
	return dist;
}

void FockState::increaseAppearance() {
	appearance_++;
}

int FockState::getAppearance() {
	return appearance_;
}

float FockState::getStateProb() {
	return probability_;
}

const std::vector<int> FockState::getState() {
	return state_;
}

void FockState::setProbability(float prob) {
	probability_ = prob;
}

void FockState::resetAppearance() {
	appearance_ = 0;
}

void FockState::printState() {
	std::cout << "[";
	for (int i : state_)
		std::cout << i << " ";
	std::cout << "]\n";
}
