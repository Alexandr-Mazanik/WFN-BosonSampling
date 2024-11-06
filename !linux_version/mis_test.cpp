
#include "bs-StateSpace.h"

#include <iostream>

std::random_device rd;
std::mt19937 generator(rd());

int main() {

	int ph_num = 4;
	int dim_num = 16;
	int batch_size = 50000;

	int tau_burn = 10;
	int tau_thin = 5;

	StateSpace spaceBF(ph_num, dim_num);
	StateSpace spaceMIS(ph_num, dim_num, false);
	StateSpace spaceDS(ph_num, dim_num, false);
	
	Scheme scheme(spaceDS);
	scheme.ImportSchemeUnitary("scheme_unitary_4.txt");

	std::vector<int> init_conf(ph_num, 1);
	init_conf.resize(dim_num);
	
	BFbosonSampler sampler_bf(&spaceBF, scheme, init_conf);
	MISampler sampler_mis(&spaceMIS, scheme, init_conf, tau_burn, tau_thin);
	DistinguishableSampler sampler_ds(&spaceDS, scheme, init_conf);
	 
	sampler_bf.sample(batch_size);
	sampler_mis.sample(batch_size);
	sampler_ds.sample(batch_size);

	spaceBF.ExportSampleData("bf_4_16_tau5_50000.txt");
	spaceMIS.ExportSampleData("mis_4_16_tau5_50000.txt");
	spaceDS.ExportSampleData("ds_4_16_tau5_50000.txt");

	return 0;
}
