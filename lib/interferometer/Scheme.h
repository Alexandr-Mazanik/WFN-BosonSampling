#pragma once

#include "space/StateSpace.h"

#include <complex>

class Scheme {
public:
	std::vector<std::vector<std::complex<float>>> unitary_matrix;

	Scheme(StateSpace& space);
	
	void ImportSchemeUnitary(std::string file_name = "scheme_unitary");

	
	void PrintSchemeMatrix();

private:
	int modes_num_;
};

