
#include <bs-StateSpace.h>

#include <iostream>

int main() {

	int ph_num = 5;
	int dim_num = 25;
	int batch_size = 200000;

	int tau_burn = 50;
	int tau_thin = 50;

	//StateSpace spaceBF(ph_num, dim_num);
	//StateSpace spaceMIS(ph_num, dim_num, false);
	StateSpace spaceDS(ph_num, dim_num, false);
	
	Scheme scheme(spaceDS);
	scheme.ImportSchemeUnitary("scheme_unitary_5.txt");

	std::vector<int> init_conf(ph_num, 1);
	init_conf.resize(dim_num);
	
	//BFbosonSampler sampler_bf(&spaceBF, scheme, init_conf);
	//MISampler sampler_mis(&spaceMIS, scheme, init_conf, tau_burn, tau_thin);
	DistinguishableSampler sampler_ds(&spaceDS, scheme, init_conf);
	 
	//sampler_bf.sample(batch_size);
	//sampler_mis.sample(batch_size);
	sampler_ds.sample(batch_size);

	//spaceBF.ExportSampleData("mis_test/data/bf_7_49_200.txt");
	//spaceMIS.ExportSampleData("mis_test/data/mis_10_100_200.txt");
	spaceDS.ExportSampleData("mis_test/data/ds_5_25_200000.txt");

	return 0;
}
