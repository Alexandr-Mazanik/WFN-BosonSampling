
#include <bs-StateSpace.h>

#include <iostream>

int main() {
	int N_start = 1;
	int N_end = 200;
	int N_step = 10;

	int avg_amount = 100;

	int ph_num = 3;
	int dim_number = ph_num * ph_num;
	int radius = 2;

	StateSpace spaceBS(ph_num, dim_number);
	StateSpace spaceUF(spaceBS);
	StateSpace spaceCH(ph_num, dim_number, false);
	StateSpace spaceDS(ph_num, dim_number, false);

	Scheme scheme(spaceBS);
	scheme.ImportSchemeUnitary("scheme_unitary_3.txt");

	std::vector<int> init_conf(ph_num, 1);
	init_conf.resize(dim_number);

	std::vector<std::complex<float>> init_conf_coherent(ph_num, 1);
	init_conf_coherent.resize(dim_number);

	std::vector<Sampler*> samplers = { 
		new BFbosonSampler(&spaceBS, scheme, init_conf), 
		new CoherentSampler(&spaceCH, scheme, init_conf_coherent), 
		new UniformSampler(&spaceUF),
		new DistinguishableSampler(&spaceDS, scheme, init_conf) 
	};

	std::vector<float> all_cloud_mu, all_cloud_sigma;
	std::vector<float> avg_mu, avg_sigma;

	for (Sampler* sampler : samplers) {
		for (int N = N_start; N <= N_end; N += N_step) {
			std::cout << "\n--> points_number = " << N << ";\n";

			for (int i = 0; i < avg_amount; ++i) {
				sampler->sample(N);
				Network network(sampler->space_ptr(), radius);
				std::vector<float> two_moments = network.degDistTwoMoments(network.degreeDistribution());

				all_cloud_mu.push_back(two_moments[0]);
				all_cloud_sigma.push_back(two_moments[1]);

				sampler->space_ptr()->reset();
			}

			avg_mu.push_back(get_vec_avg(all_cloud_mu));
			avg_sigma.push_back(get_vec_avg(all_cloud_sigma));

			all_cloud_mu.clear();
			all_cloud_sigma.clear();	
		}
		
		std::string file_name_mu = "cloud_position_N/data/cloud_mu_N_" + std::to_string(N_start) + "_" + 
			std::to_string(N_end) + "_" + std::to_string(N_step) + "_" + sampler->get_name() + ".txt";
		std::string file_name_sigma = "cloud_position_N/data/cloud_sigma_N_" + std::to_string(N_start) + "_" + 
			std::to_string(N_end) + "_" + std::to_string(N_step) + "_" + sampler->get_name() + ".txt";

		export_vec_to_file<float>(avg_mu, file_name_mu);
		export_vec_to_file<float>(avg_sigma, file_name_sigma);

		avg_mu.clear();
		avg_sigma.clear();
	}

	return 0;
}
