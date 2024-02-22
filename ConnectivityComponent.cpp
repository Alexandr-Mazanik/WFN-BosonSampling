#include "ConnectivityComponent.h"

#include <cmath>

void ConnectivityComponent::AddVertex(FockState& vertex) {
	vertices_.push_back(vertex);
}

float ConnectivityComponent::calcAVGLogProb() {
	float all_prob = 0.;
	for (FockState& state : vertices_)
		all_prob += -std::log(state.getStateProb());

	return all_prob / vertices_.size();
}

float ConnectivityComponent::calcLogProbDispersion() {
	float all_prob_sq = 0.;
	for (FockState& state : vertices_)
		all_prob_sq += std::log(state.getStateProb()) * std::log(state.getStateProb());
	all_prob_sq /= vertices_.size();
	float avg_prob = calcAVGLogProb();

	return all_prob_sq - avg_prob * avg_prob;
}

int ConnectivityComponent::getVerticesNum() {
	return vertices_.size();
}
