
#include "bs-StateSpace.h"

#include <iostream>

std::random_device rd;
std::mt19937 generator(rd());

int main() {
	int N_start = 1;
	int N_end = 2001;
	int N_step = 200;

	int avg_amount = 20;

	int ph_num = 6;
	int dim_number = 36;
	int radius = 4;

	StateSpace spaceBS(ph_num, dim_number);
	StateSpace spaceDS(ph_num, dim_number, false);
	StateSpace spaceMF(ph_num, dim_number, false);

	Scheme scheme_init(spaceBS);
	scheme_init.ImportSchemeUnitary("diff_u_6/scheme_unitary_6_0.txt");

	std::vector<int> init_conf(ph_num, 1);
	init_conf.resize(dim_number);

	std::vector<Sampler*> samplers = {
		new BFbosonSampler(&spaceBS, scheme_init, init_conf),
		new DistinguishableSampler(&spaceDS, scheme_init, init_conf),
		new MeanFieldSampler(&spaceMF, scheme_init, init_conf)
	};

	std::vector<std::vector<float>> mu_data;
	std::vector<std::vector<float>> sigma_data;

	for (Sampler* sampler : samplers) {
		std::cout << "\nSAMPLER: " << sampler->get_name() << "\n";

		mu_data.resize((N_end - N_start) / N_step + 1);
		sigma_data.resize((N_end - N_start) / N_step + 1);

		for (int i = 0; i < avg_amount; ++i) {
			Scheme scheme(spaceBS);
			std::string scheme_name = "diff_u_6/scheme_unitary_6_" + std::to_string(i) + ".txt";
			scheme.ImportSchemeUnitary(scheme_name);
			sampler->change_scheme(scheme);

			int point_num = 0, totalN = 0;
			for (int N = N_start; N <= N_end; N += N_step) {
				std::cout << i << " -- N: " << N << " / " << N_end << "\r" << std::flush;

				sampler->sample(N - totalN);
				Network network(sampler->space_ptr(), radius, false);
				std::vector<float> two_moments = network.degDistTwoMoments(network.degreeDistribution());

				mu_data[point_num].push_back(two_moments[0]);
				sigma_data[point_num].push_back(two_moments[1]);

				point_num++;
				totalN = N;

			}

			sampler->space_ptr()->reset();
		}

		std::string file_name_mu = "cloud_mu_6_diffU_N_res_" + std::to_string(N_start) + "_" +
			std::to_string(N_end) + "_" + std::to_string(N_step) + "_" + sampler->get_name() + ".txt";
		std::string file_name_sigma = "cloud_sigma_6_diffU_N_res_" + std::to_string(N_start) + "_" +
			std::to_string(N_end) + "_" + std::to_string(N_step) + "_" + sampler->get_name() + ".txt";

		export_data(mu_data, file_name_mu);
		export_data(sigma_data, file_name_sigma);

		mu_data.clear();
		sigma_data.clear();

		std::cout << "\n--> OK\n";
	}

	return 0;
}
