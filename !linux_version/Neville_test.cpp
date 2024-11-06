
#include "bs-StateSpace.h"

#include <iostream>

std::random_device rd;
std::mt19937 generator(rd());

int main() {
	int N_start = 1;
	int N_end = 18001;
	int N_step = 900;

	int avg_amount = 4;

	int ph_num = 20;
	int dim_number = 400;
	int radius = 40;

	StateSpace spaceBS(ph_num, dim_number,false);
	StateSpace spaceMIS(ph_num, dim_number, false);
	StateSpace spaceRJ(ph_num, dim_number, false);
	StateSpace spaceDS(ph_num, dim_number, false);
	StateSpace spaceMIS1000(ph_num, dim_number, false);

	std::vector<std::vector<float>> mu_data;
	std::vector<std::vector<float>> sigma_data;

	std::vector<StateSpace*> spaces = { 
		//&spaceBS, 
		&spaceMIS, 
		&spaceMIS1000, 
		&spaceDS 
	};

	std::vector<std::string> spaces_name = { 
		//"bf", 
		"mis", 
		"mis1000", 
		"ds" 
	};

	for (int space_num = 0; space_num < 3; ++space_num) {
		std::cout << "\nSPACE: " << spaces_name[space_num] << "\n";

		mu_data.resize((N_end - N_start) / N_step + 1);
		sigma_data.resize((N_end - N_start) / N_step + 1);

		for (int i = 0; i < avg_amount; ++i) {

			int point_num = 0, totalN = 0;
			for (int N = N_start; N <= N_end; N += N_step) {
				std::cout << i << " -- N: " << N << " / " << N_end << "\r" << std::flush;

				std::string file_path = "/neville_" + std::to_string(ph_num) +
					"_" + spaces_name[space_num] + "_batch" + std::to_string(N_step) +
					"/" + std::to_string(i) + std::to_string(point_num) + ".txt";

				//std::cout << file_path << std::endl;

				if (point_num == 0)
					spaces[space_num]->ImportSampleData(1, file_path);
				else
					spaces[space_num]->ImportSampleData(N_step, file_path);

				Network network(spaces[space_num], radius, false);
				std::vector<float> two_moments = network.degDistTwoMoments(network.degreeDistribution());

				mu_data[point_num].push_back(two_moments[0]);
				sigma_data[point_num].push_back(two_moments[1]);

				point_num++;
				totalN = N;

			}

			spaces[space_num]->reset();
		}

		std::string file_name_mu = "cloud_mu_20_neville_N_r40_" + std::to_string(N_start) + "_" +
			std::to_string(N_end) + "_" + std::to_string(N_step) + "_" + spaces_name[space_num] + ".txt";
		std::string file_name_sigma = "cloud_sigma_20_neville_N_r40_" + std::to_string(N_start) + "_" +
			std::to_string(N_end) + "_" + std::to_string(N_step) + "_" + spaces_name[space_num] + ".txt";

		export_data(mu_data, file_name_mu);
		export_data(sigma_data, file_name_sigma);

		mu_data.clear();
		sigma_data.clear();

		std::cout << "\n--> OK\n";
	}

	return 0;
}


