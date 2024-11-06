#pragma once

#include <vector>

class FockState {
public:
	FockState(std::vector<int>& state, int dim_num);
	
	int L1Distance(const FockState& other);
	void increaseAppearance();

	int getAppearance();
	float getStateProb();
	const std::vector<int> getState();
	
	void setProbability(float prob);
	void resetAppearance();

	void printState();

private:
	int appearance_, dim_num_;
	float probability_;
	std::vector<int> state_;
};

