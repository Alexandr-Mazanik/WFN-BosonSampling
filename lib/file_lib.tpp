#pragma once

#include <fstream>
#include <iostream>

template <typename T>
void export_vec_to_file(std::vector<T> vec, std::string file_name) {
	std::ofstream f_data;

	f_data.open("../../../analysis/" + file_name);

	if (f_data.is_open()) {
		for (T elem : vec)
			f_data << elem << " "; 
		std::cout << "\n--> Exported: " << file_name;
	}
	else
		std::cout << "\n--> Couldn't open file\n";
}

template <typename T>
void export_data(std::vector<std::vector<T>>& data, std::string file_name) {
	std::ofstream f_data;

	f_data.open("../../../analysis/" + file_name);

	if (f_data.is_open()) {
		for (std::vector<T> vec : data) {
			for (T elem : vec)
				f_data << elem << " "; 
			f_data << std::endl;
		}
		std::cout << "\n--> Exported: " << file_name;
	}
	else
		std::cout << "\n--> Couldn't open file\n";
}
