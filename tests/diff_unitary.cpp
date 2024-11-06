
#include <bs-StateSpace.h>

#include <iostream>

std::random_device rd;
std::mt19937 generator(rd());

int main() {
	int N_start = 1;
	int N_end = 1001;
	int N_step = 200;

	int avg_amount = 5;

	int ph_num = 5;
	int dim_number = 25;
	int radius = 2;

	StateSpace spaceBS(ph_num, dim_number);
	StateSpace spaceDS(ph_num, dim_number, false);
	StateSpace spaceMF(ph_num, dim_number, false);

	Scheme scheme_init(spaceBS);
	scheme_init.ImportSchemeUnitary("diff_u_5/scheme_unitary_5_0.txt");

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
			std::string scheme_name = "diff_u_5/scheme_unitary_5_" + std::to_string(i) + ".txt";
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

		std::string file_name_mu = "cloud_position_N/data/cloud_mu_5_diffU_N_" + std::to_string(N_start) + "_" +
			std::to_string(N_end) + "_" + std::to_string(N_step) + "_" + sampler->get_name() + ".txt";
		std::string file_name_sigma = "cloud_position_N/data/cloud_sigma_5_diffU_N_" + std::to_string(N_start) + "_" +
			std::to_string(N_end) + "_" + std::to_string(N_step) + "_" + sampler->get_name() + ".txt";

		export_data<float>(mu_data, file_name_mu);
		export_data<float>(sigma_data, file_name_sigma);

		mu_data.clear();
		sigma_data.clear();

		std::cout << "\n--> OK\n";
	}

	return 0;
}