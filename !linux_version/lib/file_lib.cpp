#include "file_lib.h"

void export_vec_to_file(std::vector<float> vec, std::string file_name) {
	std::ofstream f_data;
	
	f_data.open("../../analysis/" + file_name);

	if (f_data.is_open()) {
		for (float elem : vec)
			f_data << elem << " "; 
		std::cout << "--> Exported: " << file_name << std::endl;
	}
	else
		std::cout << "--> Couldn't open file\n";
}

void export_data(std::vector<std::vector<float>>& data, std::string file_name) {
	std::ofstream f_data;

	f_data.open(".././analysis/" + file_name);
	
	if (f_data.is_open()) {
		for (std::vector<float> vec : data) {
			for (float elem : vec)
				f_data << elem << " "; 
			f_data << std::endl;
		}
		std::cout << "--> Exported: " << file_name << std::endl;
	}
	else
		std::cout << "--> Couldn't open file\n";
}
