#pragma once

#include "FockState.h"
#include <string>

class StateSpace {
public:
	std::vector<FockState> states;

	StateSpace(int ph_num, int dim_num, bool createBasis = true);
	StateSpace(const StateSpace& space);
	StateSpace(int dim_num, std::vector<FockState> states_to_create = {});

	void CreateFockBasis();

	void calcMeanPhotonNumber();
	float twoPointCorrFunction(int mode1, int mode2);
	
	float calcAVGLogProb();
	float calcLogProbDispersion();
	
	void ExportSampleData(std::string file_name = "sample_cpp.txt");
	void ImportSampleData(int sample_num, std::string file_name = "data.txt");
	
	void AddSampledState(std::vector<int>& sample, float prob = -1);
	void AddState(std::vector<int>& state);

	void reset();
	
	int getPhNum() const;
	int getModesNum() const;
	bool getIsBasis() const;
	int getStatesNumber();
	std::vector<FockState> getSampledStates();
	std::vector<FockState>& getAllStates();

	void printStateSpace(int points_num = 1);

private:
	int ph_num_, dim_num_;
	bool fock_basis_;
	std::vector<float> meanPhN_;
};

