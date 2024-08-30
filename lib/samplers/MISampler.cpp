#include "samplers/MISampler.h"

MISampler::MISampler(StateSpace* space, Scheme& scheme, std::vector<int>& init_state) :
	init_state_(init_state), scheme_(scheme), space_ptr_(space), 
	ph_num_((*space_ptr_).getPhNum()), modes_num_((*space_ptr_).getModesNum()),
	base_space_(ph_num_, modes_num_, false), base_sampler_(&base_space_, scheme, init_state) { }

void MISampler::sample(int batch_size) {

}


StateSpace* MISampler::space_ptr() {
	return space_ptr_;
}

std::string MISampler::get_name() {
	return "mis";
}
