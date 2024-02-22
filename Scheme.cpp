#include "Scheme.h"

#include <iostream>
#include <fstream>

Scheme::Scheme(StateSpace& space) {
	modes_num_ = space.getModesNum();
	unitary_matrix.resize(modes_num_);
}

void Scheme::ImportSchemeUnitary(std::string file_name) {
	std::ifstream f_unitary;
	std::string s_unitary;

	f_unitary.open("./data/" + file_name);

	if (f_unitary.is_open()) {
		int row_num = 0;
		while (f_unitary and row_num < modes_num_) {
			std::getline(f_unitary, s_unitary);

			int j = 0;
			std::string s_num;
			std::vector<float> values;
			while (s_unitary[j] != '\0') {
				if (s_unitary[j] != '\t') {
					s_num += s_unitary[j];
				}
				else {
					values.push_back(std::stof(s_num));
					s_num.clear();
				}
				j++;
			}
			int i = 0;
			for (int col_num = 0; col_num < modes_num_; ++col_num) {
				unitary_matrix[row_num].push_back(std::complex<float>(values[i], values[i + 1]));
				i += 2;
			}
			row_num++;
		}
		std::cout << "--> Scheme was successfully uploaded\n";
	}
	else {
		std::cout << "--> Couldn't open file\n";
	}
}

void Scheme::PrintSchemeMatrix() {
	for (std::vector<std::complex<float>> vec : unitary_matrix) {
		for (std::complex<float> num : vec)
			std::cout << num << " ";
		std::cout << std::endl;
	}
}
