#pragma once

#include <fstream>
#include <iostream>

template <typename T>
void export_vec_to_file(std::vector<T> vec, std::string file_name) {
	std::ofstream f_data;

	f_data.open("../../../analysis/" + file_name);

	if (f_data.is_open())
		for (T elem : vec)
			f_data << elem << " ";
	else
		std::cout << "--> Couldn't open file\n";
}
