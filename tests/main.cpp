#include "samplers/BFbosonSampler.h"
#include "samplers/UniformSampler.h"
#include "samplers/CoherentSampler.h"
#include "samplers/DistinguishableSampler.h"

#include "network/Network.h"

#include <iostream>
#include <fstream>
#include <string>

template <typename T>
void export_vec_to_file(std::vector<T> vec, std::string file_name) {
	std::ofstream f_data;

	f_data.open("./data/" + file_name);

	if (f_data.is_open())
		for (T elem : vec)
			f_data << elem << " ";
	else
		std::cout << "--> Couldn't open file\n";
}

float calc_mean_vec(std::vector<float> vec) {
	float total = 0;
	for (float elem : vec) 
		total += elem;
	
	return total / vec.size();
}

int main() {
	int points_num = 1000;

	int ph_num = 4;
	int dim_number = ph_num * ph_num;
	int radius = 2;

	StateSpace spaceBS(ph_num, dim_number);
	StateSpace spaceUF(spaceBS);
	StateSpace spaceCH(ph_num, dim_number, false);
	StateSpace spaceDS(ph_num, dim_number, false);
	
	Scheme scheme(spaceBS);
	scheme.ImportSchemeUnitary("scheme_unitary_4.txt");

	std::vector<int> init_conf(ph_num, 1);
	init_conf.resize(dim_number);

	std::vector<std::complex<float>> init_conf_coherent(ph_num, 1);
	init_conf_coherent.resize(dim_number);

	BFbosonSampler sampler_bf(&spaceBS, scheme, init_conf);
	UniformSampler sampler_uf(&spaceUF);
	CoherentSampler sampler_ch(&spaceCH, scheme, init_conf_coherent);
	DistinguishableSampler sampler_ds(&spaceDS, scheme, init_conf);


	std::vector<float> all_means_bs;
	std::vector<float> all_means_uf;
	std::vector<float> all_means_ch;
	std::vector<float> all_means_ds;

	std::vector<float> all_dispersions_bs;
	std::vector<float> all_dispersions_uf;
	std::vector<float> all_dispersions_ch;
	std::vector<float> all_dispersions_ds;

	std::vector<float> bs_moments;
	std::vector<float> uf_moments;
	std::vector<float> ch_moments;
	std::vector<float> ds_moments;

	std::vector<float> means_bs;
	std::vector<float> means_uf;
	std::vector<float> means_ch;
	std::vector<float> means_ds;

	std::vector<float> disp_bs;
	std::vector<float> disp_uf;
	std::vector<float> disp_ch;
	std::vector<float> disp_ds;

	for (int p_num = points_num; p_num <= 1002; p_num += 20) {
		for (int i = 0; i < 100; ++i) {
			std::cout << "\n--> points_number = " << p_num << ";\n";
			std::cout << "\n--> i = " << i << ";\n";

			sampler_bf.sample(p_num);
			sampler_uf.sample(p_num);
			sampler_ch.sample(p_num);
			sampler_ds.sample(p_num);

			Network network_bs(spaceBS, radius);
			Network network_uf(spaceUF, radius);
			Network network_ch(spaceCH, radius);
			Network network_ds(spaceDS, radius);

			bs_moments = network_bs.degDistTwoMoments(network_bs.degreeDistribution());
			uf_moments = network_uf.degDistTwoMoments(network_uf.degreeDistribution());
			ch_moments = network_ch.degDistTwoMoments(network_ch.degreeDistribution());
			ds_moments = network_ds.degDistTwoMoments(network_ds.degreeDistribution());

			all_means_bs.push_back(bs_moments[0]);
			all_means_uf.push_back(uf_moments[0]);
			all_means_ch.push_back(ch_moments[0]);
			all_means_ds.push_back(ds_moments[0]);

			all_dispersions_bs.push_back(bs_moments[1]);
			all_dispersions_uf.push_back(uf_moments[1]);
			all_dispersions_ch.push_back(ch_moments[1]);
			all_dispersions_ds.push_back(ds_moments[1]);

			spaceBS.reset();
			spaceUF.reset();
			spaceCH.reset();
			spaceDS.reset();
			bs_moments.clear();
			uf_moments.clear();
			ch_moments.clear();
			ds_moments.clear();
		}

		means_bs.push_back(calc_mean_vec(all_means_bs));
		means_uf.push_back(calc_mean_vec(all_means_uf));
		means_ch.push_back(calc_mean_vec(all_means_ch));
		means_ds.push_back(calc_mean_vec(all_means_ds));

		disp_bs.push_back(calc_mean_vec(all_dispersions_bs));
		disp_uf.push_back(calc_mean_vec(all_dispersions_uf));
		disp_ch.push_back(calc_mean_vec(all_dispersions_ch));
		disp_ds.push_back(calc_mean_vec(all_dispersions_ds));

		all_means_bs.clear();
		all_means_uf.clear();
		all_means_ch.clear();
		all_means_ds.clear();
		all_dispersions_bs.clear();
		all_dispersions_uf.clear();
		all_dispersions_ch.clear();
		all_dispersions_ds.clear();
	}

	/*
	export_vec_to_file<float>(means_bs, "means_bs_3_large_N.txt");
	export_vec_to_file<float>(means_uf, "means_uf_3_large_N.txt");
	export_vec_to_file<float>(means_ch, "means_ch_3_large_N.txt");
	export_vec_to_file<float>(means_ds, "means_ds_3_large_N.txt");

	export_vec_to_file<float>(disp_bs, "disp_bs_3_large_N.txt");
	export_vec_to_file<float>(disp_uf, "disp_uf_3_large_N.txt");
	export_vec_to_file<float>(disp_ch, "disp_ch_3_large_N.txt");
	export_vec_to_file<float>(disp_ds, "disp_ds_3_large_N.txt");
	*/

	return 0;
}
