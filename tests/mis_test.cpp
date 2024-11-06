
#include <bs-StateSpace.h>

#include <iostream>

std::random_device rd;
std::mt19937 generator(rd());

int main() {

	int ph_num = 4;
	int dim_num = 16;
	int batch_size = 10;

	int tau_burn = 3;
	int tau_thin = 3;

	//StateSpace spaceBF(ph_num, dim_num);
	StateSpace spaceMIS(ph_num, dim_num, false);
	//StateSpace spaceDS(ph_num, dim_num, false);
	
	Scheme scheme(spaceMIS);
	scheme.ImportSchemeUnitary("scheme_unitary_4.txt");

	std::vector<int> init_conf(ph_num, 1);
	init_conf.resize(dim_num);
	
	//BFbosonSampler sampler_bf(&spaceBF, scheme, init_conf);
	MISampler sampler_mis(&spaceMIS, scheme, init_conf, tau_burn, tau_thin);
	//DistinguishableSampler sampler_ds(&spaceDS, scheme, init_conf);
	 
	//sampler_bf.sample(batch_size);
	sampler_mis.sample(batch_size);
	//sampler_ds.sample(batch_size);

	//spaceBF.ExportSampleData("mis_test/data/bf_7_49_200.txt");
	//spaceMIS.ExportSampleData("mis_test/data/mis_10_100_200.txt");
	//spaceDS.ExportSampleData("mis_test/data/ds_5_25_200000.txt");

	return 0;
}
