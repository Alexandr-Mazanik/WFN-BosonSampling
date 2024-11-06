#include "../network/Network.h"
#include "../space/StateSpace.h"

#include <iostream>
#include <fstream>
#include <stack>

Network::Network(StateSpace* space_ptr, int radius, bool make_report) : 
	networkSpace_(StateSpace(space_ptr->getModesNum(), space_ptr->getSampledStates())) {

	vertices_num_ = networkSpace_.getStatesNumber();

	network_.resize(vertices_num_, std::vector<bool>(vertices_num_));
	BuildNetwork(radius, make_report);
};

Network::Network(const Network& network) : networkSpace_(network.networkSpace_) {
	vertices_num_ = networkSpace_.getStatesNumber();

	network_ = network.network_;
}

Network::Network(StateSpace* space_ptr) : 
	networkSpace_(StateSpace(space_ptr->getModesNum(), {})) {

	vertices_num_ = networkSpace_.getStatesNumber();
}

void Network::BuildNetwork(int radius, bool make_report) {
	for (int i = 0; i < vertices_num_; ++i) {
		network_[i][i] = false;
		for (int j = i + 1; j < vertices_num_; ++j)
			if (networkSpace_.states[i].L1Distance(networkSpace_.states[j]) <= radius)
				network_[i][j] = network_[j][i] = true;
			else
				network_[i][j] = network_[j][i] = false;
	}
	if (make_report)
		std::cout << "--> The network has been built\n";
}

void Network::countConnectedComponents() {
	std::vector<bool> visited(vertices_num_, 0);
	for (int i = 0; i < vertices_num_; ++i)
		if (!visited[i]) 
			DFS(i, visited);
}

void Network::DFS(int vertex, std::vector<bool>& visited) {
	Network con_comp(&networkSpace_);
	std::stack<int> st;
	st.push(vertex);

	while (!st.empty()) {
		int curr = st.top();
		st.pop();
		if (!visited[curr]) {
			visited[curr] = true;

			std::vector<int> state = networkSpace_.states[curr].getState();
			con_comp.networkSpace_.AddState(state);
			for (int i = network_[curr].size() - 1; i >= 0; i--)
				if (network_[curr][i])
					st.push(i);
		}
	}

	con_comp_.push_back(con_comp);
}

std::vector<int> Network::componentsDistribution() {
	countConnectedComponents();

	std::vector<int> dist_vert_num;
	for (int i = 0; i < con_comp_.size(); ++i)
		for (int j = 0; j < con_comp_[i].networkSpace_.getStatesNumber(); ++j)
			dist_vert_num.push_back(con_comp_[i].networkSpace_.getStatesNumber());

	return dist_vert_num;
}

std::vector<float> Network::degreeDistribution() {
	std::vector<float> prob_list(1, 0);
	int k_max = 0;

	for (int i = 0; i < vertices_num_; ++i) {
		int k = 0;
		for (bool edge : network_[i])
			k += (int)edge;

		if (k > k_max) {
			k_max = k;
			prob_list.resize(k_max + 1, 0);
		}
		prob_list[k] ++;
	}

	for (int i = 0; i <= k_max; ++i)
		prob_list[i] = prob_list[i] / vertices_num_;

	return prob_list;
}

std::vector<float> Network::degDistTwoMoments(const std::vector<float>& distr) {
	std::vector<float> two_moments = { 0, 0 };

	for (int i = 0; i < distr.size(); ++i)
		two_moments[0] += distr[i] * i;

	for (int i = 0; i < distr.size(); ++i)
		two_moments[1] += distr[i] * (i - two_moments[0]) * (i - two_moments[0]);

	return two_moments;
}

std::vector<Network>& Network::getConComp() {
	return con_comp_;
}

void Network::printNetwork() {
	std::cout << "--> Network:\n";
	for (int i = 0; i < networkSpace_.getStatesNumber(); ++i) {
		for (int j = 0; j < networkSpace_.getStatesNumber(); ++j) {
			std::cout << network_[i][j] << " ";
		}
		std::cout << std::endl;
	}
}
