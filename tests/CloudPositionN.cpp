
#include <bs-StateSpace.h>

#include <iostream>

int main() {
	int N_start = 1;
	int N_end = 2001;
	int N_step = 200;

	int avg_amount = 5;

	int ph_num = 6;
	int dim_number = 36;
	int radius = 6;

	StateSpace spaceBS(ph_num, dim_number);
	StateSpace spaceUF(spaceBS);
	StateSpace spaceCH(ph_num, dim_number, false);
	StateSpace spaceDS(ph_num, dim_number, false);
	StateSpace spaceMF(ph_num, dim_number, false);

	Scheme scheme(spaceBS);
	scheme.ImportSchemeUnitary("scheme_unitary_6.txt");

	std::vector<int> init_conf(ph_num, 1);
	init_conf.resize(dim_number);
	
	std::vector<std::complex<float>> init_conf_coherent(ph_num, 1);
	init_conf_coherent.resize(dim_number);

	std::vector<Sampler*> samplers = { 
		new BFbosonSampler(&spaceBS, scheme, init_conf), 
		//new CoherentSampler(&spaceCH, scheme, init_conf_coherent), 
		//new UniformSampler(&spaceUF),
		new DistinguishableSampler(&spaceDS, scheme, init_conf),
		new MeanFieldSampler(&spaceMF, scheme, init_conf)
	};

	//std::vector<float> all_cloud_mu, all_cloud_sigma;
	std::vector<std::vector<float>> mu_data, sigma_data;

	for (Sampler* sampler : samplers) {
		std::cout << "\nSAMPLER: " << sampler->get_name() << "\n";
		for (int N = N_start; N <= N_end; N += N_step) {
			std::cout << "N: " << N << " / " << N_end << "\r";

			mu_data.resize(mu_data.size() + 1);
			sigma_data.resize(sigma_data.size() + 1);

			for (int i = 0; i < avg_amount; ++i) {
				sampler->sample(N);
				Network network(sampler->space_ptr(), radius, false);
				std::vector<float> two_moments = network.degDistTwoMoments(network.degreeDistribution());

				mu_data.back().push_back(two_moments[0]);
				sigma_data.back().push_back(two_moments[1]);

				sampler->space_ptr()->reset();
			}

			//avg_mu.push_back(get_vec_avg(all_cloud_mu));
			//avg_sigma.push_back(get_vec_avg(all_cloud_sigma));

			//all_cloud_mu.clear();
			//all_cloud_sigma.clear();	
		}
		
		std::string file_name_mu = "cloud_position_N/data/cloud_mu_6_r6_N_" + std::to_string(N_start) + "_" + 
			std::to_string(N_end) + "_" + std::to_string(N_step) + "_" + sampler->get_name() + ".txt";
		std::string file_name_sigma = "cloud_position_N/data/cloud_sigma_6_r6_N_" + std::to_string(N_start) + "_" + 
			std::to_string(N_end) + "_" + std::to_string(N_step) + "_" + sampler->get_name() + ".txt";

		export_data<float>(mu_data, file_name_mu);
		export_data<float>(sigma_data, file_name_sigma);

		mu_data.clear();
		sigma_data.clear();

		std::cout << "\n--> OK\n";
	}

	return 0;
}
